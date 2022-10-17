#ifndef HSM_H
#define HSM_H

#include <cstdint>

struct Event;
class State;

class HSM
{
public:
    void Init(const Event *event = nullptr);
    void Dispatch(const Event *event);
    void ChangeState(State *mainTargetState) { mCurrentState = mainTargetState; }

protected:
    HSM(State *initial) : mCurrentState(initial) {}

    static class TopState sTopState;
private:
    State *mCurrentState;
};

enum class Signal : uint8_t;

struct Event
{
    Signal sig;
};

enum class Ret : uint8_t
{
    IGNORED, HANDLED, TRANSITION 
};

class State
{
public:
    virtual ~State() = default;

    virtual void OnEntry(HSM *context) const {}
    virtual void OnExit(HSM *context) const {}
    virtual Ret OnEvent(HSM *context, const Event *event) const = 0;

    bool OnInitial(HSM *context) const
    {
        if (mInitial)
        {
            context->ChangeState(mInitial);
            return true;
        }

        return false;
    }

    void SetParent(State *parent) { mParent = parent; }
    State *GetParent() const { return mParent; }

    void SetInitial(State *initial) { mInitial = initial; }

protected:
    State(const std::string &name) : mName(name), mParent(nullptr), mInitial(nullptr) {}

private:
    std::string mName;
    State *mParent;
    State *mInitial;
};

class TopState : public State
{
public:
    Ret OnEvent(HSM *context, const Event *event) const override
    {
        return Ret::IGNORED;
    }
};

#endif // HSM_H