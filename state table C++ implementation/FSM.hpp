#include "StateTable.hpp"
#include <iostream>

struct TickingEvent : Event
{
    TickingEvent(Signal signal, uint8_t initialFineTime, uint8_t currentFineTime) : Event{signal}, initialFineTime(initialFineTime), currentFineTime(currentFineTime) {}
    uint8_t initialFineTime;
    uint8_t currentFineTime;
};

enum class State : uint8_t { SETTING_STATE, TICKING_STATE };
enum class Signal : uint8_t { PRESS_UP_EVENT, PRESS_DOWN_EVENT, ARM_EVENT, TICK_EVENT };

Event Up{ Signal::PRESS_UP_EVENT };
Event Down{ Signal::PRESS_DOWN_EVENT };
Event Arm{ Signal::ARM_EVENT };
TickingEvent Tick(Signal::TICK_EVENT, 5, 5);

class BombFSM : public StateTable
{
public:
    BombFSM(uint8_t timeout, uint8_t defuseCode) : 
    StateTable(StateHandlerTable{ 
                                    { static_cast<StateHandler>(&BombFSM::SettingStateUpEventHandler), static_cast<StateHandler>(&BombFSM::SettingStateDownEventHandler), static_cast<StateHandler>(&BombFSM::SettingStateArmEventHandler), static_cast<StateHandler>(&BombFSM::EmptyStateEventHandler) },
                                    { static_cast<StateHandler>(&BombFSM::TickingStateUpEventHandler), static_cast<StateHandler>(&BombFSM::TickingStateDownEventHandler), static_cast<StateHandler>(&BombFSM::TickingStateArmEventHandler), static_cast<StateHandler>(&BombFSM::TickingStateTickEventHandler) }
                                },  static_cast<StateHandler>(&BombFSM::InitialTransition)), 
    mTimeout(timeout), mDefuseCode(defuseCode), mCode(0) {}
private:
    uint8_t mTimeout;
    uint8_t mDefuseCode;
    uint8_t mCode;

    void InitialTransition(Event *event)
    {
        (void)event;

        mCurrentStateIndex = (uint8_t)State::SETTING_STATE;         

        std::cout << "adjust bomb timeout" << std::endl;
    }

    void SettingStateUpEventHandler(Event *event)
    {
        (void)event;

        if (mTimeout < 60)
            mTimeout++;

        std::cout << "timeout: " << (int)mTimeout << std::endl;
    }

    void SettingStateDownEventHandler(Event *event)
    {
        (void)event;

        if (mTimeout > 1)
            mTimeout--;

        std::cout << "timeout: " << (int)mTimeout << std::endl;
    }

    void SettingStateArmEventHandler(Event *event)
    {
        (void)event;

        mCurrentStateIndex = (uint8_t)State::TICKING_STATE;
        std::cout << "bomb armed\n";
    }

    void TickingStateUpEventHandler(Event *event)
    {
        (void)event;

        mCode <<= 1;
        mCode |= 1;

        std::cout << "current code: " << (int)mCode << std::endl;
    }

    void TickingStateDownEventHandler(Event *event)
    {
        (void)event;

        mCode <<= 1;

        std::cout << "current code: " << (int)mCode << std::endl;
    }

    void TickingStateArmEventHandler(Event *event)
    {
        (void)event;

        if (mCode == mDefuseCode)
        {
            std::cout << "bomb defused\n";
            mCurrentStateIndex = (uint8_t)State::SETTING_STATE;
        }    
        else
            std::cout << "wrong code, bomb still ticking...\n";
    }

    void TickingStateTickEventHandler(Event *event)
    {
        TickingEvent *tickingEvent = (TickingEvent*)event;       

        tickingEvent->currentFineTime--;
        if (tickingEvent->currentFineTime == 255)
        {   
            tickingEvent->currentFineTime = tickingEvent->initialFineTime;
            mTimeout--;
        }

        std::cout << "timeout: " << (int)mTimeout << "." << (int)tickingEvent->currentFineTime << std::endl;

        if (mTimeout == 0 && tickingEvent->currentFineTime == 0)
        {
            std::cout << "BOOOM, bomb exploded\n";
            mCurrentStateIndex = (uint8_t)State::SETTING_STATE;
            std::cout << "adjust bomb timeout" << std::endl;
        }


    }
};