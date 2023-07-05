#ifndef HFSM_H
#define HFSM_H

#include <cstdint>
#include <map>
#include <string>
#include <cassert>
#include "../../data structures/ADT/Stack/Stack.hpp"

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
        // std::string result;

        // for (auto statePair : mStates)
        //     if (statePair.second == mCurrentState)
        //     {
        //         result = statePair.first;
        //         break;
        //     }

        // return result;

        // C++17 structured bindings
        for (auto const &[stateName, state] : mStates)
            if (state == mCurrentState)
                return stateName;

        return "";
    }

    void Init()
    {
        HFSMState *rootState = nullptr;

        // 1. find the root state (single enclosing state without parent)

        // C++17 structured bindings
        for (auto [stateName, state] : mStates)
            if (!state->GetParent())
            {
                rootState = state;
                break;
            }

        // for (auto statePair : mStates)
        //     if (!statePair.second->GetParent())
        //     {
        //         rootState = statePair.second;
        //         break;
        //     }

        assert(rootState);

        // 2. enter root state

        rootState->OnEnter();

        // 3. enter initial state configuration
        Stack<HFSMState*> targetStateConfiguration;
        HFSMState *state = rootState;

        while (state->GetInitialState())   // find target leaf state
            state = state->GetInitialState();
            
        while (state != rootState)   // save states from target leaf to root state excluded
        {
            targetStateConfiguration.Push(state);
            state = state->GetParent();
        }

        while (!targetStateConfiguration.Empty())
        {
            mCurrentState = targetStateConfiguration.Top();
            mCurrentState->OnEnter();
            targetStateConfiguration.Pop();
        }
    }

    void Update(float deltaTime) 
    {
        mCurrentState->OnUpdate(deltaTime);
    }

private:
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
            HFSMState *mainTargetAncestor = mainTargetState;
            //HFSMState *mainTargetAncestor = mainTargetState->GetParent();

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
            mCurrentState->OnEnter();
            targetStateConfiguration.Pop();
        }
    }
};

#endif // HFSM_H