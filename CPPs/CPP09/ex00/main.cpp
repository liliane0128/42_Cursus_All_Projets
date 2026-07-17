#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: ./btc <input_file>.\n";
		return 1;
	}
	BitcoinExchange btc;
	if (!btc.loadDatabase("data.csv"))
		return 1;
	if (!btc.processInput(av[1]))
		return 1;
	return 0;
}