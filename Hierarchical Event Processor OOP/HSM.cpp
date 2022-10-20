#include "hsm.hpp"
#include <vector>

TopState HSM::sTopState;
Initial HSM::initial;
S HSM::s;
S1 HSM::s1;
S11 HSM::s11;
S2 HSM::s2;
S21 HSM::s21;
S211 HSM::s211;

HSM::HSM()
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

    mCurrentState = &initial;
}

void HSM::Init(const Event *event)
{
    Ret ret = mCurrentState->OnEvent(this, event);

    //if (ret != Ret::TRANSITION)
    //   ; error

    State *initialSourceState = &sTopState;  // top state is the root of all states
    
    do
    {
        std::vector<State*> path;

        // get the path from the target of initial transition to the source of initial transition
        State *state = mCurrentState;
        while (state != initialSourceState)
        {
            path.push_back(state);
            state = state->GetParent();
        }

        // enter all the states from the source of initial transition to the target of initial transition
        for (std::vector<State*>::reverse_iterator rit = path.rbegin(); rit != path.rend(); ++rit)
            (*rit)->OnEntry(this);

        initialSourceState = mCurrentState;    // the source of the new initial transition is the current state
    } while (mCurrentState->OnInitial(this));  // trigger the new initila transition. if leaf state exit loop
}

void HSM::Dispatch(const Event *event)
{
    State *oldState = mCurrentState;

    // find the state that handles the event
    State *state = mCurrentState;
    Ret ret;
    while (state && (ret = state->OnEvent(this, event)) == Ret::IGNORED)
        state = state->GetParent();

    if (state && ret == Ret::TRANSITION)  // event triggered transition
    {
        State *mainSource = state;                
        State *mainTarget = mCurrentState;

        // exit all states up until the main source state
        state = oldState;
        while (state != mainSource)
        {
            state->OnExit(this);
            state = state->GetParent();
        }

        // find the Least Common Ancestor (LCA), the lowest state in the hierarchy which is superstate of both main source and main target states
        // (if the main source state or the main target state are superstate of each other the superstate is the LCA)
        State *LCA = nullptr;
        if (mainSource == mainTarget)         // corner case: self transition
            LCA = mainSource->GetParent();
        else
        {
            State *sourceState = mainSource;
            while (sourceState)
            {
                State *targetState = mainTarget;
                while (targetState && targetState != sourceState)
                    targetState = targetState->GetParent();

                if (targetState)
                {
                    LCA = targetState;
                    break;
                }

                sourceState = sourceState->GetParent();
            }
        }

        // if (!LCA)
        //     ; error

        // exit all states from the main source state up until the LCA. don't exit the LCA
        state = mainSource;
        while (state != LCA)
        {
            state->OnExit(this);
            state = state->GetParent();
        }

        // find the path from the LCA down to the main target state
        std::vector<State*> path;
        state = mainTarget;
        while (state != LCA)
        {
            path.push_back(state);
            state = state->GetParent();
        }

        // enter all states from the state below the LCA to the target state
        for (auto rit = path.rbegin(); rit != path.rend(); ++rit)
            (*rit)->OnEntry(this);

        // use the initial transitions to enter the leaf state 
        State *initialSource = mainTarget;
        while (initialSource->OnInitial(this))
        {
            State *initialTarget = mCurrentState;

            std::vector<State*> path;
            state = initialTarget;
            while (state != initialSource)
            {
                path.push_back(state);
                state = state->GetParent();
            }

            for (auto rit = path.rbegin(); rit != path.rend(); ++rit)
                (*rit)->OnEntry(this);
    
            initialSource = initialTarget;
        }
    }
}

Ret Initial::OnEvent(HSM *context, const Event *event) const 
{
    (void)event;
    context->mFoo = 0;
    context->ChangeState(&HSM::s2);

    return Ret::TRANSITION;
}

Ret S::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::I:
            if (context->mFoo)
            {
                context->mFoo = 0;
                return Ret::HANDLED;
            }
            break;

        case Signal::E:
            context->ChangeState(&HSM::s11);
            return Ret::TRANSITION;           
    }
    
    return Ret::IGNORED; 
}

Ret S1::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::A:
            context->ChangeState(&HSM::s1);
            return Ret::TRANSITION;

        case Signal::B:
            context->ChangeState(&HSM::s11);
            return Ret::TRANSITION;

        case Signal::C:
            context->ChangeState(&HSM::s2);
            return Ret::TRANSITION;

        case Signal::D:
            if (!context->mFoo)
            {   
                context->mFoo = 1;
                context->ChangeState(&HSM::s);
                return Ret::TRANSITION;
            }
            break;

        case Signal::F:
            context->ChangeState(&HSM::s211);
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
            if (context->mFoo)
            {
                context->mFoo = 0;
                context->ChangeState(&HSM::s1);
                return Ret::TRANSITION;
            }
            break;

        case Signal::G:
            context->ChangeState(&HSM::s211);
            return Ret::TRANSITION;

        case Signal::H:
            context->ChangeState(&HSM::s);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}

Ret S2::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {
        case Signal::C:
            context->ChangeState(&HSM::s1);
            return Ret::TRANSITION;

        case Signal::F:
            context->ChangeState(&HSM::s11);
            return Ret::TRANSITION;

		case Signal::I:
			if (!context->mFoo)
			{
				context->mFoo = 1;
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
            context->ChangeState(&HSM::s21);
            return Ret::TRANSITION;

        case Signal::B:
            context->ChangeState(&HSM::s211);
            return Ret::TRANSITION;

        case Signal::G:
            context->ChangeState(&HSM::s11);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}

Ret S211::OnEvent(HSM *context, const Event *event) const 
{
    switch (event->sig)
    {   
        case Signal::D:
            context->ChangeState(&HSM::s21);
            return Ret::TRANSITION;

        case Signal::H:
            context->ChangeState(&HSM::s);
            return Ret::TRANSITION;
    }

    return Ret::IGNORED;
}