#include "FSM.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    int timeout;
    std::cout << "select bomb timeout: ";
    std::cin >> timeout;

    int defuseCode;
    std::cout << "select bomb defuse code: ";
    std::cin >> defuseCode;

    BombFSM bomb(timeout, defuseCode);

    bomb.Init();

    uint8_t signal;
    while (std::cin)
    {
        std::cin >> signal;

        switch (signal)
        {
            case 'u':
                bomb.Dispatch(&Up);
                break;
            case 'd':
                bomb.Dispatch(&Down);
                break;
            case 'a':
                bomb.Dispatch(&Arm);
                break;
            case 't':
                bomb.Dispatch(&Tick);
                break;
            default:
                std::cout << "invalid signal\n";
        }
    }

    return EXIT_SUCCESS;
}