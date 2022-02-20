#ifndef STATE_H
#define STATE_H

class Event;

class StateHandler
{
public:
    virtual void operator()(const Event&) = 0;
protected:
    StateHandler() = default;
};

template <typename T, typename E>
class StateEventHandler : public StateHandler
{
public:
    StateEventHandler(T *instance, void (T::*ptrToMemFun)(const E&)) : mInstance(instance), mPtrToMemFun(ptrToMemFun) {}
    void operator()(const Event &event) override { (mInstance->*mPtrToMemFun)(static_cast<const E&>(event)); }
private:
    T *mInstance;
    void (T::*mPtrToMemFun)(const E&);
};

#endif  // STATE_H