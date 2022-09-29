#ifndef STATE_TABLE_H
#define STATE_TABLE_H

#include <cstdint>
#include <vector>

enum class Signal : uint8_t;

struct Event
{
    Signal signal;
};

class StateTable
{
protected:
    using StateHandler = void (StateTable::*)(Event*);
    using StateHandlerTable = std::vector<std::vector<StateHandler>>;
public:
    StateTable(StateHandlerTable stateHandlerTable, StateHandler initialTransition) : mStateHandlers(stateHandlerTable), mInitialTransition(initialTransition) {}

    void Init()
    {
        (this->*mInitialTransition)(nullptr);
    }

    void Dispatch(Event *event)
    {
        (this->*mStateHandlers[mCurrentStateIndex][(uint8_t)event->signal])(event);
    }
private:
    StateHandlerTable mStateHandlers;
    StateHandler mInitialTransition;
protected:
    uint8_t mCurrentStateIndex;
    void EmptyStateEventHandler(Event *event) { (void)event; }
};

#endif // STATE_TABLE_H