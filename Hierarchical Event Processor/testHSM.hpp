#ifndef TEST_HSM_H
#define TEST_HSM_H

#include "HSM.hpp"
#include <iostream>

#define PRINT(msg)     (std::cout << msg)
#define PRINTLN(msg)   (PRINT(msg) << '\n')

enum class TestSignal : uint8_t
{
    A = static_cast<uint8_t>(Signal::USER_SIG), B, C, D, E, F, G, H, I
};

struct TestSubEvent1 : Event
{
    uint8_t param;
};

class TestHSM : public HSM
{
public:
    TestHSM() : HSM(static_cast<StateHandler>(&TestHSM::Initial)) {}
    
private:
    State Initial(Event const *event)
    {
        foo = 0;
        return TRANSITION(&TestHSM::S2);
    }
    
    State S(const Event *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S:EXIT");
            return IGNORED();

        case Signal::INITIAL:
            return TRANSITION(&TestHSM::S11);

        case static_cast<Signal>(TestSignal::I):
            if (foo)
            {
                foo = 0;
                return HANDLED();
            }
            break;

        case static_cast<Signal>(TestSignal::E):
            return TRANSITION(&TestHSM::S11);
        }

        return SUPER(&HSM::TopStateHandler);       
    }
    
    State S1(Event const *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S1:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S1:EXIT");
            return IGNORED();

        case Signal::INITIAL:
            return TRANSITION(&TestHSM::S11);

        case static_cast<Signal>(TestSignal::A):
            return TRANSITION(&TestHSM::S1);

        case static_cast<Signal>(TestSignal::B):
            return TRANSITION(&TestHSM::S11);

        case static_cast<Signal>(TestSignal::C):
            return TRANSITION(&TestHSM::S2);

        case static_cast<Signal>(TestSignal::D):
            if (!foo)
            {   
                foo = 1;
                return TRANSITION(&TestHSM::S);
            }
            break;

        case static_cast<Signal>(TestSignal::F):
            return TRANSITION(&TestHSM::S211);
        
        case static_cast<Signal>(TestSignal::I):
            return HANDLED();   
        }

        return SUPER(&TestHSM::S);
    }
   
    State S11(const Event *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S11:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S11:EXIT");
            return IGNORED();

		case Signal::INITIAL:
			return IGNORED();

        case static_cast<Signal>(TestSignal::D):
            if (foo)
            {
                foo = 0;
                return TRANSITION(&TestHSM::S1);
            }
            break;

        case static_cast<Signal>(TestSignal::G):
            return TRANSITION(&TestHSM::S211);
            break;

        case static_cast<Signal>(TestSignal::H):
            return TRANSITION(&TestHSM::S);
            break;
        }

        return SUPER(&TestHSM::S1);
    }

    State S2(Event const *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S2:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S2:EXIT");
            return IGNORED();

        case Signal::INITIAL:
            return TRANSITION(&TestHSM::S211);

        case static_cast<Signal>(TestSignal::C):
            return TRANSITION(&TestHSM::S1);

        case static_cast<Signal>(TestSignal::F):
            return TRANSITION(&TestHSM::S11);

        case static_cast<Signal>(TestSignal::I):
            if (!foo)
            {
                foo = 1;
                return HANDLED();
            }
            break;
        }

        return SUPER(&TestHSM::S);
    }

    State S21(Event const *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S21:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S21:EXIT");
            return IGNORED();

        case Signal::INITIAL:
            return TRANSITION(&TestHSM::S211);

        case static_cast<Signal>(TestSignal::A):
            return TRANSITION(&TestHSM::S21);

        case static_cast<Signal>(TestSignal::B):
            return TRANSITION(&TestHSM::S211);

        case static_cast<Signal>(TestSignal::G):
            return TRANSITION(&TestHSM::S11);
        }

        return SUPER(&TestHSM::S2);
    }

    State S211(Event const *event)
    {
        switch (event->sig)
        {
        case Signal::ENTRY:
            PRINTLN("S211:ENTRY");
            return IGNORED();

        case Signal::EXIT:
            PRINTLN("S211:EXIT");
            return IGNORED();

		case Signal::INITIAL:
            return IGNORED();

        case static_cast<Signal>(TestSignal::D):
            return TRANSITION(&TestHSM::S21);

        case static_cast<Signal>(TestSignal::H):
            return TRANSITION(&TestHSM::S);
        }

        return SUPER(&TestHSM::S21);
    }

    int foo;
};

#endif //  TEST_HSM_H