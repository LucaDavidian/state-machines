#ifndef FSM_H
#define FSM_H

#include "event.hpp"  // event contains an enum - can't be specialized

class FSM
{
protected:
    typedef void (FSM::*State)(const Event&);
public:  
    void DispatchEvent(Event &event) { (this->*mState)(event); }
protected:
    FSM(State initialPseudoState) :mState(initialPseudoState) {}
    void Init() { (this->*mState)(Event{ Event::Signal::RESERVED }); }  // initial pseudostate transistions to initial state
    void SetState(State state);
private:
    State mState;
};

class FSMImplementation : public FSM
{
public:
    FSMImplementation() : FSM(static_cast<State>(&FSMImplementation::InitialState)) { Init(); }
private:
    void InitialState(const Event &event);
    void StateA(const Event &event);
    void StateB(const Event &event);
    void StateC(const Event &event);
};

#endif  // FSM_H