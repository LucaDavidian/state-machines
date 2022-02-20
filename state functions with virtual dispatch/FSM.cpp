#include "FSM.hpp"

void FSM::SetState(State state) 
{ 
    if (mState != state)
    {
        (*mState)(Event{2}); 

        mState = state; 
    
        (*mState)(Event{1});
    }
}

