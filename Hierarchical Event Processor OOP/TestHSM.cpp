#include "testHSM.hpp"

Initial TestHSM::initial;
S TestHSM::s;
S1 TestHSM::s1;
S11 TestHSM::s11;
S2 TestHSM::s2;
S21 TestHSM::s21;
S211 TestHSM::s211;

TestHSM::TestHSM() : HSM(&initial) 
    {
        s11.SetParent(&s1);
        s1.SetParent(&s);
        s1.SetInitial(&s11);

        s211.SetParent(&s21);
        s21.SetParent(&s2);
        s21.SetInitial(&s211);
        s2.SetParent(&s);
        s2.SetInitial(&s211);

        s.SetParent(&sTopState);
        s.SetInitial(&s11);
    }

Ret Initial::OnEvent(HSM *context, const Event *event) const 
{
    (void)event;
    static_cast<TestHSM*>(context)->mFoo = 0;
    context->ChangeState(&TestHSM::s2);

    return Ret::TRANSITION;
}

Ret S::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::I:
            if (static_cast<TestHSM*>(context)->mFoo)
            {
                static_cast<TestHSM*>(context)->mFoo = 0;
                return Ret::HANDLED;
            }
            break;

        case Signal::E:
            context->ChangeState(&TestHSM::s11);
            return Ret::TRANSITION;           
    }
    
    return Ret::IGNORED; 
}

Ret S1::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::A:
            context->ChangeState(&TestHSM::s1);
            return Ret::TRANSITION;

        case Signal::B:
            context->ChangeState(&TestHSM::s11);
            return Ret::TRANSITION;

        case Signal::C:
            context->ChangeState(&TestHSM::s2);
            return Ret::TRANSITION;

        case Signal::D:
            if (!static_cast<TestHSM*>(context)->mFoo)
            {   
                static_cast<TestHSM*>(context)->mFoo = 1;
                context->ChangeState(&TestHSM::s);
                return Ret::TRANSITION;
            }
            break;

        case Signal::F:
            context->ChangeState(&TestHSM::s211);
            return Ret::TRANSITION;
        
        case Signal::I:
            return Ret::HANDLED;         
    }
    
    return Ret::IGNORED;  
}

Ret S11::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::D:
            if (static_cast<TestHSM*>(context)->mFoo)
            {
                static_cast<TestHSM*>(context)->mFoo = 0;
                context->ChangeState(&TestHSM::s1);
                return Ret::TRANSITION;
            }
            break;

        case Signal::G:
            context->ChangeState(&TestHSM::s211);
            return Ret::TRANSITION;

        case Signal::H:
            context->ChangeState(&TestHSM::s);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}

Ret S2::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::C:
            context->ChangeState(&TestHSM::s1);
            return Ret::TRANSITION;

        case Signal::F:
            context->ChangeState(&TestHSM::s11);
            return Ret::TRANSITION;

		case Signal::I:
			if (!static_cast<TestHSM *>(context)->mFoo)
			{
				static_cast<TestHSM *>(context)->mFoo = 1;
				return Ret::HANDLED;
			}
			break;
    }

    return Ret::IGNORED;
}

Ret S21::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {   
        case Signal::A:
            context->ChangeState(&TestHSM::s21);
            return Ret::TRANSITION;

        case Signal::B:
            context->ChangeState(&TestHSM::s211);
            return Ret::TRANSITION;

        case Signal::G:
            context->ChangeState(&TestHSM::s11);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}

Ret S211::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {   
        case Signal::D:
            context->ChangeState(&TestHSM::s21);
            return Ret::TRANSITION;

        case Signal::H:
            context->ChangeState(&TestHSM::s);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}