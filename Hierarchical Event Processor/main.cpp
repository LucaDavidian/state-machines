#include "testHSM.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
    TestHSM hsm;
    hsm.Init();

    do 
    {
        char c;
        std::cout << "enter event: ";
        std::cin >> c;
        std::cout << '\n';

        TestSignal signal;
        switch (c)
        {
            case 'A':
                signal = TestSignal::A;
                break;
			case 'B':
				signal = TestSignal::B;
				break;
			case 'C':
				signal = TestSignal::C;
				break;
			case 'D':
				signal = TestSignal::D;
				break;
			case 'E':
				signal = TestSignal::E;
				break;
			case 'F':
				signal = TestSignal::F;
				break;
			case 'G':
				signal = TestSignal::G;
				break;
			case 'H':
				signal = TestSignal::H;
				break;
			case 'I':
				signal = TestSignal::I;
				break;
			default:
				std::cout << "enter valid event: from A to I" << '\n';
				continue;
        }

        Event event{ static_cast<Signal>(signal) };
        hsm.Dispatch(&event);

    } while (std::cin);
    
    return EXIT_SUCCESS;
}