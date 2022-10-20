#ifndef HSM_H
#define HSM_H

#include <cstdint>
#include <string>
#include <iostream>

#define PRINT(msg)     (std::cout << msg)
#define PRINTLN(msg)   (PRINT(msg) << '\n')

// events
enum class Signal : uint8_t
{
    A, B, C, D, E, F, G, H, I
};

struct Event
{
    Signal sig;
};

class State;

// HSM context class 
class HSM
{
friend class Initial;
friend class S;
friend class S1;
friend class S11;
friend class S2;
friend class S21;
friend class S211;

public:
    HSM();

    void Init(const Event *event = nullptr);
    void Dispatch(const Event *event);
    void ChangeState(State *mainTargetState) { mCurrentState = mainTargetState; }
    
private:
    State *mCurrentState;   // state variable

    int mFoo;              // extended state variable
    
    static class TopState sTopState;   // top pseudo state

    static class Initial initial;      // HSM states
    static class S s;
    static class S1 s1;
    static class S11 s11;
    static class S2 s2;
    static class S21 s21;
    static class S211 s211;
};

// OnEvent return values
enum class Ret : uint8_t
{
    IGNORED, HANDLED, TRANSITION 
};

// state abstract base class
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

// concrete states
class TopState : public State
{
public:
    TopState() : State("top state") {}

    Ret OnEvent(HSM *context, const Event *event) const override
    {
        return Ret::IGNORED;
    }
};

class Initial : public State
{
public:
    Initial() : State("initial") {}

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S : public State
{
public:
    S() : State("S") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S1 : public State
{
public:
    S1() : State("S1") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S1:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S1:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S11 : public State
{
public:
    S11() : State("S11") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S11:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S11:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S2 : public State
{
public:
    S2() : State("S2") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S2:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S2:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S21 : public State
{
public:
    S21() : State("S21") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S21:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S21:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class S211 : public State
{
public:
    S211() : State("2S11") {}

    void OnEntry(HSM *context) const override
    {
        PRINTLN("S211:ENTRY");
    }

    void OnExit(HSM *context) const override
    {
        PRINTLN("S211:EXIT");
    }

    Ret OnEvent(HSM *context, const Event *event) const override;
};

#endif // HSM_H