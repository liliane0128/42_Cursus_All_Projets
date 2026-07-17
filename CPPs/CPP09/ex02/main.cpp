#include "PmergeMe.hpp"

int main(int ac, char **av)
{
	try
	{
		PmergeMe data;
		data.parseInput(ac, av);
		data.execute();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}