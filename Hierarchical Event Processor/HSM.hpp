#ifndef HSM_H
#define HSM_H

#include <cstdint>
#include <cassert>

enum class Signal : uint8_t 
{
    EMPTY, ENTRY, EXIT, INITIAL, USER_SIG
};

struct Event
{
    Signal sig;
};

Event ReservedEvents[] = {
    Event{ Signal::EMPTY }, Event{ Signal::ENTRY }, Event{ Signal::EXIT }, Event{ Signal::INITIAL }
};

#define TRIG_SIGNAL(state, sig) ((this->*state)(&ReservedEvents[static_cast<uint8_t>(sig)]))

enum class State
{
    HANDLED, IGNORED, TRANSITION, SUPER
};

#define TRANSITION(state) (Transition(static_cast<StateHandler>(state)), State::TRANSITION)
#define HANDLED()  (State::HANDLED)
#define IGNORED()  (State::IGNORED)
#define SUPER(state) (Transition(static_cast<StateHandler>(state)), State::SUPER)

#include <vector>

class HSM
{
public:
    typedef State (HSM::*StateHandler)(Event const *event);   // states are represented as state-handler functions
   
    void Init(const Event *event = nullptr)
    {
        StateHandler initialSourceStateHandler = &HSM::TopStateHandler;   // initial transition from top state
        
        State ret = (this->*mCurrentStateHandler)(event);         // take initial transition (set as current state in ctor) - now current state handler is target of initial transition
        assert(ret == State::TRANSITION);                         // initial pseudostate - event must cause a transition of state
        
        do
        {
            std::vector<StateHandler> path;
            StateHandler initialTargetStateHandler = mCurrentStateHandler;  // current state is target of initial transition

            while (initialTargetStateHandler != initialSourceStateHandler)
            {
                path.push_back(initialTargetStateHandler);               
                TRIG_SIGNAL(initialTargetStateHandler, Signal::EMPTY);   
                initialTargetStateHandler = mCurrentStateHandler;    
            } 

            mCurrentStateHandler = path.front();       // restore current state handler as target of initial transition  

            // std::vector<StateHandler>::size_type i = path.size() - 1;
            // while (i >= 0)
            // {
            //     TRIG_SIGNAL(path[i], Signal::ENTRY);
            //     i--;     
            // }

            for (std::vector<StateHandler>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it)  // enter all states until the target of initial transition
                TRIG_SIGNAL(*it, Signal::ENTRY);

            
            initialSourceStateHandler = mCurrentStateHandler;  // source of new initial transition is the target state of previous initial transition
            
        } while (TRIG_SIGNAL(mCurrentStateHandler, Signal::INITIAL) == State::TRANSITION);  // current state handler is target of nested initial transition
    }  

    void Dispatch(const Event *event)
    {
        StateHandler oldStateHandler = mCurrentStateHandler;  // save current state
        StateHandler tempStateHandler = mCurrentStateHandler;
        State ret;
      
        while ((ret = (this->*mCurrentStateHandler)(event)) == State::SUPER)  // find the state that handles the event or the source state of the transition
            tempStateHandler = mCurrentStateHandler;

        // if the event has been handled (ret == State::HANDLED) or ignored (ret == State::IGNORED) go to last statement to reset the current state to the old state

        if (ret == State::TRANSITION)   
        {
            StateHandler sourceStateHandler = tempStateHandler;        // if the event triggered a transition, tempStateHandler contains the main source state
            StateHandler targetStateHandler = mCurrentStateHandler;    // and mCurrentStateHandlerhandler contains the main target state 

            std::vector<StateHandler> path;       // stores the path to enter (in reverse order) to reach the target state
            int pathIndex = -1;                   // the index into the path array from which to enter the path to the target state
            path.push_back(targetStateHandler);   // path[0] is the main target of transition
                     
            /* 1. exit all states up to the source state */

            tempStateHandler = oldStateHandler;
            while (tempStateHandler != sourceStateHandler)
            {
                if (TRIG_SIGNAL(tempStateHandler, Signal::EXIT) != State::SUPER)    // if state handles exit action use empty signal to retrieve the super state
                    (void)TRIG_SIGNAL(tempStateHandler, Signal::EMPTY);             // otherwise a state handler returns SUPER
                
                tempStateHandler = mCurrentStateHandler;
            }  

            /* 2. exit all states from the source state up to the least common ancestor (LCA) of the source state and the target state
                  (the lowest state in the hierarchy that is superstate of both the source and target state). Don't exit the LCA */

            if (sourceStateHandler == targetStateHandler)  // A. self transition
            {
                (void)TRIG_SIGNAL(sourceStateHandler, Signal::EXIT);  // exit source
                pathIndex = 0;                                        // enter target
            }
            else  
            {
                (void)TRIG_SIGNAL(targetStateHandler, Signal::EMPTY);
                StateHandler targetSuperStateHandler = mCurrentStateHandler;

                if (sourceStateHandler == targetSuperStateHandler)           // B. source state is direct superstate of target state
                    pathIndex = 0;  // enter target
                else
                {
                    (void)TRIG_SIGNAL(sourceStateHandler, Signal::EMPTY);
                    StateHandler sourceSuperStateHandler = mCurrentStateHandler;

                    if (sourceSuperStateHandler == targetSuperStateHandler)    // C. source and target states have the same superstate (siblings)
                    {
                        (void)TRIG_SIGNAL(sourceStateHandler, Signal::EXIT);   // exit source
                        pathIndex = 0;                                         // enter target
                    }
                    else
                    {
                        if (targetStateHandler == sourceSuperStateHandler)        // D. target state is direct superstate of source state
                            (void)TRIG_SIGNAL(sourceStateHandler, Signal::EXIT);  // exit source
                        else   
                        {
                            path.push_back(targetSuperStateHandler);
                            pathIndex = 1;  // enter superstate of target and target
                            bool LCAFound = false;

                            while (TRIG_SIGNAL(targetSuperStateHandler, Signal::EMPTY) == State::SUPER)
                            {
                                targetSuperStateHandler = mCurrentStateHandler;
                                path.push_back(targetSuperStateHandler);
                                pathIndex++;

                                if (targetSuperStateHandler == sourceStateHandler)  // E. source state is indirect superstate of target state
                                {
                                    LCAFound = true;
                                    pathIndex--;    // do not enter the source
                                    break;
                                }
                            }

                            if (!LCAFound)   // F. LCA is direct superstate of source and indirect superstate of target
                            {
                                (void)TRIG_SIGNAL(sourceStateHandler, Signal::EXIT);  // exit source state
                                                               
                                int i = path.size() - 1;   // now path contains all superstates of target up unitl the top state
                                while (i >= 0)
                                    if (sourceSuperStateHandler == path[i])
                                    {
                                        LCAFound = true;
                                        pathIndex = i - 1;  // do not enter LCA
                                        break;
                                    }
                                    else
                                        i--;
                            

                                if (!LCAFound)  // G. LCA is indirect superstate of both source and target & H. target state is indirect superstate of source state
                                {
                                    do
                                    {
                                        if ((ret = TRIG_SIGNAL(sourceSuperStateHandler, Signal::EXIT)) != State::SUPER)
                                            ret = TRIG_SIGNAL(sourceSuperStateHandler, Signal::EMPTY);

                                        sourceSuperStateHandler = mCurrentStateHandler;

                                        int i = path.size() - 1;
                                        while (i >= 0)
                                        {
                                            if (sourceSuperStateHandler == path[i])
                                            {
                                                LCAFound = true;
                                                pathIndex = i - 1;  // do not enter LCA
                                                break;
                                            }
                                            else
                                                i--;
                                        }

                                        if (LCAFound)           
                                            break;
                                    } while (ret == State::SUPER);
                                }

                            }
                        }
                    }

                }
            }

            /* 3. enter the states starting from the state below the LCA down to the target state using initial transitions */
            while (pathIndex >= 0)
            {
                TRIG_SIGNAL(path[pathIndex], Signal::ENTRY);
                pathIndex--;
            }

            mCurrentStateHandler = path[0];  // set target state as current state

            /* 4. enter the states below the target state configuration using initial transitions down to the leaf state */           
            StateHandler initialSourceStateHandler = mCurrentStateHandler;     // source of initial transition
            
            while (TRIG_SIGNAL(mCurrentStateHandler, Signal::INITIAL) == State::TRANSITION)
            {
                StateHandler initialTargetStateHandler = mCurrentStateHandler;  // current state is target of initial transition

                std::vector<StateHandler> path;

                while (initialTargetStateHandler != initialSourceStateHandler)  // push all superstates of the target of initial transition up to the source state
                {
                    path.push_back(initialTargetStateHandler);
                    TRIG_SIGNAL(initialTargetStateHandler, Signal::EMPTY);
                    initialTargetStateHandler = mCurrentStateHandler;
                }

                mCurrentStateHandler = path.front();  // restore current state to target of last initial transition

                for (std::vector<StateHandler>::reverse_iterator it = path.rbegin(); it != path.rend(); ++it)  // enter all states until the target of initial transition
                    TRIG_SIGNAL(*it, Signal::ENTRY);

                initialSourceStateHandler = mCurrentStateHandler;  // start again from target of last initial transition until a leaf state is reached
            }
            
        }
        else
            mCurrentStateHandler = oldStateHandler;   // set the new state or restore the old state
        
    }

    bool IsIn(StateHandler stateHandler)
    {
        StateHandler currentStateHandler = mCurrentStateHandler;  // save current state
        
        do   // if the HSM is in a state, it is at the same time in every superstate of that state
        {
            if (mCurrentStateHandler == stateHandler)
            {
                mCurrentStateHandler = currentStateHandler;  // restore state
                return true;
            }
        } while (TRIG_SIGNAL(mCurrentStateHandler, Signal::EMPTY) != State::IGNORED);
        
        return false;
    }

    State TopStateHandler(const Event *event)    // top state handler
    {
        (void)event;
        return IGNORED();
    }
    
protected:
    HSM(StateHandler initial) : mCurrentStateHandler(initial) {}
    
    void Transition(StateHandler stateHandler)
    {
        mCurrentStateHandler = stateHandler;
    }

private:
    StateHandler mCurrentStateHandler;       // current state
};

#endif  // HSM_H