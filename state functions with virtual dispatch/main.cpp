#include "FSMImplementation.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    FSMImplementation fsm;

    Event event;
    
    char c;
    do
    {
        std::cin >> c;
    
        if (c == '1')
            event.signal = EventImpl::SIG1;
        else if (c == '2')
            event.signal = EventImpl::SIG2;
        else if (c == '3')
            event.signal = EventImpl::SIG3;
        else
            continue;
            
        fsm.DispatchEvent(event);
    } while (std::cin);

    return 0;
}