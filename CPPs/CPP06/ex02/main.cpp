#include "functions.hpp"

int main()
{
    std::srand(std::time(NULL));
    for (int i = 0; i < 8; i++)
    {
        Base* instance = generate();
        if (instance)
		{
			std::cout << GREEN << "Pointer identify  : " << RESET;
			identify(instance);

			std::cout <<YELLOW << "Reference identify: " << RESET;
        	identify(*instance);
		}

        delete instance;
    }
    return 0;
}