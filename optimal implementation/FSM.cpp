#include "FSM.hpp"

void FSM::SetState(State state) 
{ 
    if (mState != state)
    {
        (this->*mState)(Event{Event::Signal::EXIT}); 

        mState = state; 
    
        (this->*mState)(Event{Event::Signal::ENTRY});
    }
}

#include <iostream>

#define PRINT(s)    std::cout << (s)
#define PRINTLN(s)  PRINT(s) << std::endl

void FSMImplementation::InitialState(const Event &event)
{
	if (event.signal == Event::Signal::RESERVED)
        SetState(static_cast<State>(&FSMImplementation::StateA));
}

void FSMImplementation::StateA(const Event &event)
{
    switch (event.signal)
    {
        case (Event::Signal::ENTRY):
            PRINTLN("entering state A");
            break;

        case (Event::Signal::EXIT):
            PRINTLN("leaving state A");
            break;

        case (Event::Signal::SIG2):
            SetState(static_cast<State>(&FSMImplementation::StateB));
            break;

        case (Event::Signal::SIG3):
            SetState(static_cast<State>(&FSMImplementation::StateC));
            break;
    }      
}

void FSMImplementation::StateB(const Event &event)
{
    switch (event.signal)
    {
        case (Event::Signal::ENTRY):
            PRINTLN("entering state B");
            break;

        case (Event::Signal::EXIT):
            PRINTLN("leaving state B");
            break;

        case (Event::Signal::SIG1):
            SetState(static_cast<State>(&FSMImplementation::StateC));
            break;

        case (Event::Signal::SIG3):
            SetState(static_cast<State>(&FSMImplementation::StateB));
            break;
    }
}

void FSMImplementation::StateC(const Event &event)
{
    switch (event.signal)
    {
        case (Event::Signal::ENTRY):
            PRINTLN("entering state C");
            break;

        case (Event::Signal::EXIT):
            PRINTLN("leaving state C");
            break;

        case (Event::Signal::SIG2):
            SetState(static_cast<State>(&FSMImplementation::StateA));
            break;

        case (Event::Signal::SIG3):
            SetState(static_cast<State>(&FSMImplementation::StateB));
            break;
    }
}
