#ifndef HFSM_H
#define HFSM_H

#include <cstdint>
#include <map>
#include <type_traits>
#include <string>
#include <vector>
#include <cassert>
#include "../../data structures/ADT/Stack/Stack.hpp"

// using StateId = uint8_t;

// namespace Details 
// {
//     StateId GetUniqueId()
//     {
//         static StateId stateId = 0U;

//         return stateId++;
//     }

//     template <typename T>
//     StateId GetStateId()
//     {
//         static StateId stateId = GetUniqueId();

//         return stateId;
//     }

// }   // namespace Details



class HFSMState
{
public:
    virtual void OnEnter() = 0;
    virtual void OnExit() = 0;

    virtual void OnUpdate(float deltaTime) 
    {
        if (mParent)
            mParent->OnUpdate(deltaTime);  // default behavior: 'pass the buck' (chain of responsibility pattern)
    }

    void SetInitialState(HFSMState *initialState)
    {
        mInitialState = initialState;
    }

    HFSMState *GetInitialState() const 
    {
        return mInitialState;
    }

    void SetParent(HFSMState *parent)
    {
        mParent = parent;
    }

    HFSMState *GetParent() const 
    {
        return mParent;
    }

protected:
    HFSMState(class HFSM *hfsm) : mHFSM(hfsm) {}

    HFSM *mHFSM;

private:
    HFSMState *mParent = nullptr;
    HFSMState *mInitialState = nullptr;
};



class HFSM
{
public:
    // template <typename S>
    // void AddState()
    // {
    //     static_assert(std::is_base_of<HFSMState, S>::value);   
                                                              
    //     mStates.insert({Details::GetStateId<S>(), new S});    
    // } 

    // template <typename S>
    // void ChangeState()
    // {
    //     if (auto it = mStates.find(Details::GetStateId<S>()); it != mStates.end())
    //     {
    //         HFSMState *mainTargetState = it->second;  // get target state configuration

            
    //     }
    // }                                                       
                                                             
    void AddState(const std::string &stateName, HFSMState *state)  // or create state object with reflection and store in
    {                                                              // std::map<std::string, State*>
        mStates.insert(std::make_pair(stateName, state));           
    }

    void ChangeState(HFSMState *mainSourceState, const std::string &mainTargetStateName)
    {
        if (auto it = mStates.find(mainTargetStateName); it != mStates.end())
        {
            HFSMState *mainTargetState = it->second;      // get main target state 
            Transition(mainSourceState, mainTargetState);
        }
    }

    std::string GetCurrentStateName() const
    {
        std::string result;

        for (auto it : mStates)
            if (it.second == mCurrentState)
                return it.first;

        return result;
    }

    void Init()
    {
        HFSMState *initialState = nullptr;

        for (auto statePair : mStates)
            if (!statePair.second->GetParent())
            {
                initialState = statePair.second;
                break;
            }

        assert(initialState);

        mCurrentState = initialState;
        mCurrentState->OnEnter();

        while (mCurrentState->GetInitialState())
        {
            mCurrentState = mCurrentState->GetInitialState();
            mCurrentState->OnEnter();
        }
    }

    void Update(float deltaTime) 
    {
        mCurrentState->OnUpdate(deltaTime);
    }

private:
    //std::map<StateId, HFSMState*> mStates;
    std::map<std::string, HFSMState*> mStates;  // state-transition table

    HFSMState *mCurrentState = nullptr;

    void Transition(HFSMState *mainSourceState, HFSMState *mainTargetState)
    {
        // 1. find the least common ancestor of the main source state and the main target state
        
        HFSMState *LCA = nullptr;

        HFSMState *mainSourceAncestor = mainSourceState->GetParent();
        //HFSMState *mainSourceAncestor = mainSourceState;

        while (mainSourceAncestor)
        {
            //HFSMState *mainTargetAncestor = mainTargetState->GetParent();
            HFSMState *mainTargetAncestor = mainTargetState;

            while (mainTargetAncestor && mainSourceAncestor != mainTargetAncestor)
                mainTargetAncestor = mainTargetAncestor->GetParent();

            if (mainSourceAncestor == mainTargetAncestor)
            {
                LCA = mainSourceAncestor;
                break;
            }

            mainSourceAncestor = mainSourceAncestor->GetParent();
        }

        assert(LCA);

        // 2. exit the source state configuration up to the LCA excluded

        while (mCurrentState && mCurrentState != LCA)
        {
            mCurrentState->OnExit();
            mCurrentState = mCurrentState->GetParent();
        }

        assert(mCurrentState);

        // 3. enter the target state configuration starting from below the LCA

        Stack<HFSMState*> targetStateConfiguration;

        HFSMState *state = mainTargetState;
        
        while (state->GetInitialState())   // find target leaf state
            state = state->GetInitialState();
            
        while (state != LCA)   // save states from target leaf to LCA excluded
        {
            targetStateConfiguration.Push(state);
            state = state->GetParent();
        }

        while (!targetStateConfiguration.Empty())
        {
            mCurrentState = targetStateConfiguration.Top();
            targetStateConfiguration.Pop();
            mCurrentState->OnEnter();
        }
    }
};

#endif // HFSM_H