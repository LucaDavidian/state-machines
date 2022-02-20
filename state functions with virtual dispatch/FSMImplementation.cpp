#include "FSMImplementation.hpp"
#include <iostream>

#define PRINT(s)    std::cout << (s)
#define PRINTLN(s)  PRINT(s) << std::endl

FSMImplementation::FSMImplementation() : mInitialState(new StateEventHandler<FSMImplementation, EventImpl>(this, &FSMImplementation::InitialState)), mStateA(new StateEventHandler<FSMImplementation, EventImpl>(this, &FSMImplementation::StateA)), mStateB(new StateEventHandler<FSMImplementation, EventImpl>(this, &FSMImplementation::StateB)), mStateC(new StateEventHandler<FSMImplementation, EventImpl>(this, &FSMImplementation::StateC)) 
{
    Init(mInitialState);
}

FSMImplementation::~FSMImplementation()
{
    delete mInitialState;
    delete mStateA;
    delete mStateB;
    delete mStateC;
}

void FSMImplementation::InitialState(const EventImpl &event)
{
	if (event.signal == EventImpl::RESERVED)
        SetState(mStateA);
}

void FSMImplementation::StateA(const EventImpl &event)
{
    switch (event.signal)
    {
        case (EventImpl::ENTRY):
            PRINTLN("entering state A");
            break;

        case (EventImpl::EXIT):
            PRINTLN("leaving state A");
            break;

        case (EventImpl::SIG2):
            SetState(mStateB);
            break;

        case (EventImpl::SIG3):
            SetState(mStateC);
            break;
    }      
}

void FSMImplementation::StateB(const EventImpl &event)
{
    switch (event.signal)
    {
        case (EventImpl::ENTRY):
            PRINTLN("entering state B");
            break;

        case (EventImpl::EXIT):
            PRINTLN("leaving state B");
            break;

        case (EventImpl::SIG1):
            SetState(mStateC);
            break;

        case (EventImpl::SIG3):
            SetState(mStateB);
            break;
    }
}

void FSMImplementation::StateC(const EventImpl &event)
{
    switch (event.signal)
    {
        case (EventImpl::ENTRY):
            PRINTLN("entering state C");
            break;

        case (EventImpl::EXIT):
            PRINTLN("leaving state C");
            break;

        case (EventImpl::SIG2):
            SetState(mStateA);
            break;

        case (EventImpl::SIG3):
            SetState(mStateB);
            break;
    }
}