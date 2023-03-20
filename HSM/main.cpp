#include "hsm.hpp"
#include <iostream>

char c;

class S0 : public HFSMState
{
public:
    S0(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S0" << std::endl; }
    void OnExit() override { std::cout << "exiting state S0" << std::endl; }
    void OnUpdate(float deltaTime) override
    {
        std::cout << "update state S0" << std::endl;
    }
};

class S1 : public HFSMState
{
public: 
    S1(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S1" << std::endl; }
    void OnExit() override { std::cout << "exiting state S1" << std::endl; }
    void OnUpdate(float deltaTime) override
    {
        std::cout << "update state S1" << std::endl;

        if (c == 'b') 
            mHFSM->ChangeState(this, "S21");   
        else
            HFSMState::OnUpdate(deltaTime);
    }
};

class S11 : public HFSMState
{
public:
    S11(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S11" << std::endl; }
    void OnExit() override { std::cout << "exiting state S11" << std::endl; }
    void OnUpdate(float deltaTime) override 
    {
        std::cout << "update state S11" << std::endl;

        if (c == 'a') 
            mHFSM->ChangeState(this, "S211");   
        else if (c == 'd') 
            std::cout << "state S11 internal transition" << std::endl;
        else
            HFSMState::OnUpdate(deltaTime);
    }
};

class S2 : public HFSMState
{
public:
    S2(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S2" << std::endl; }
    void OnExit() override { std::cout << "exiting state S2" << std::endl; }
    void OnUpdate(float deltaTime) override
    {
        std::cout << "update state S2" << std::endl;

        if (c == 'g')
            mHFSM->ChangeState(this, "S1");
        else     
            HFSMState::OnUpdate(deltaTime);
    }
};

class S21 : public HFSMState
{
public:
    S21(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S21" << std::endl; }
    void OnExit() override { std::cout << "exiting state S21" << std::endl; }
    void OnUpdate(float deltaTime) override
    {
        std::cout << "update state S21" << std::endl;

        if (c == 'c') 
            mHFSM->ChangeState(this, "S21"); 
        else     
            HFSMState::OnUpdate(deltaTime);
    }
};

class S211 : public HFSMState
{
public:
    S211(HFSM *hfsm) : HFSMState(hfsm) {}

    void OnEnter() override { std::cout << "entering state S211" << std::endl; }
    void OnExit() override { std::cout << "exiting state S211" << std::endl; }
    void OnUpdate(float deltaTime) override
    {
        std::cout << "update state S211" << std::endl;

        HFSMState::OnUpdate(deltaTime);
    }
};

int main(int argc, char *argv[])
{
    HFSM stateMachine;

    S211 s211(&stateMachine);
    S11 s11(&stateMachine);
    S21 s21(&stateMachine);
    S2 s2(&stateMachine);
    S1 s1(&stateMachine);
    S0 s0(&stateMachine);
    
    s0.SetInitialState(&s1);
    s1.SetInitialState(&s11);
    s2.SetInitialState(&s21);
    s21.SetInitialState(&s211);

    s1.SetParent(&s0);
    s2.SetParent(&s0);
    s11.SetParent(&s1);
    s21.SetParent(&s2);
    s211.SetParent(&s21);

    stateMachine.AddState("S0", &s0);
    stateMachine.AddState("S1", &s1);
    stateMachine.AddState("S2", &s2);
    stateMachine.AddState("S11", &s11);
    stateMachine.AddState("S21", &s21);
    stateMachine.AddState("S211", &s211);

    stateMachine.Init();

    extern char c;
    while (std::cin >> c)
    {
        stateMachine.Update(0.1f);
        std::cout << "current state: " << stateMachine.GetCurrentStateName() << std::endl;
    }

    //std::cout << "transition s1 --> s21" << std::endl;
    //stateMachine.ChangeState(&s1, "S21");

    //stateMachine.Update(0.1f);

    //std::cout << "transition s2 --> s1" << std::endl;
    //stateMachine.ChangeState(&s2, "S1");

    //stateMachine.Update(0.1f);

    return EXIT_SUCCESS;
}

