#include "testHSM.hpp"
#include <cstdlib>

int main(int argc, char *argv[])
{
    TestHSM hsm;
    hsm.Init();

	while (std::cin)
    {
        char c;
        std::cout << "select event (letters from A to I) - Q to quit" << '\n';
        std::cin >> c;

        Signal signal;
        switch (c)
        {
            case 'A':
                signal = Signal::A;
                break;
			case 'B':
				signal = Signal::B;
				break;
			case 'C':
				signal = Signal::C;
				break;
			case 'D':
				signal = Signal::D;
				break;
			case 'E':
				signal = Signal::E;
				break;
			case 'F':
				signal = Signal::F;
				break;
			case 'G':
				signal = Signal::G;
				break;
			case 'H':
				signal = Signal::H;
				break;
			case 'I':
				signal = Signal::I;
				break;
			case 'Q':
				std::cin.setstate(std::ios::failbit);
				continue;
			default:
				std::cout << "enter valid event: from A to I" << '\n';
				continue;
        }

        Event event{signal};
        hsm.Dispatch(&event);
    } 

    return EXIT_SUCCESS;
}