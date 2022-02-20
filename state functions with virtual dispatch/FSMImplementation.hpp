#include "FSM.hpp"

struct EventImpl : public Event
{   
    // signals
    enum { RESERVED = 0, ENTRY, EXIT, SIG1, SIG2, SIG3, };
    // event data
};

class FSMImplementation : public FSM
{
public:
    FSMImplementation();
    ~FSMImplementation();
private:
    void InitialState(const EventImpl &event);
    void StateA(const EventImpl &event);
    void StateB(const EventImpl &event);
    void StateC(const EventImpl &event);

    State mInitialState;
    State mStateA;
    State mStateB;
    State mStateC;
};