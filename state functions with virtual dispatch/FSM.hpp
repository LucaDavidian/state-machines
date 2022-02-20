#ifndef FSM_H
#define FSM_H

#include "state.hpp"
#include "event.hpp"

class FSM
{
protected:
    typedef StateHandler *State;
public:  
    void DispatchEvent(Event &event) { /* (*mState)(event); */ mState->operator()(event); }
protected:
    FSM() = default;
    void Init(State initialPseudoState) { mState = initialPseudoState; (*mState)(Event{0}); }  // initial pseudostate transistions to initial state
    void SetState(State state);
private:
    State mState;
};

#endif  // FSM_H