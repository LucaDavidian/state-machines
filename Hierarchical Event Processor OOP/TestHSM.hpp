#ifndef TEST_HSM_H
#define TEST_HSM_H

#include "hsm.hpp"
#include <iostream>

#define PRINT(msg)     (std::cout << msg)
#define PRINTLN(msg)   (PRINT(msg) << '\n')

class Initial : public State
{
public:
    Initial() : State("initial") {}

    Ret OnEvent(HSM *context, const Event *event) const override;
};

class TestHSM : public HSM
{
friend class Initial;
friend class S;
friend class S1;
friend class S11;
friend class S2;
friend class S21;
friend class S211;

public:
    TestHSM();

private:
    int mFoo;

    static class Initial initial;
    static class S s;
    static class S1 s1;
    static class S11 s11;
    static class S2 s2;
    static class S21 s21;
    static class S211 s211;
};

enum class Signal : uint8_t
{
    A, B, C, D, E, F, G, H, I
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

#endif // TEST_HSM_H