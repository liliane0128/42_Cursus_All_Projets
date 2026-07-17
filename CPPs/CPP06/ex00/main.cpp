#include "ScalarConverter.hpp"

// int main()
// {
// 	ScalarConverter::convert("a");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("b");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("42");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("-42");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("58.6");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("999999999999999999999");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("-9666.555f");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("nan");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("-inf");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("inff");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("");

// 	ScalarConverter::convert("qwwww");
// 	std::cout << std::endl;

// 	ScalarConverter::convert("42abc");


// 	return 0;
// }

int main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cout << "Usage: ./ScalarConverter < >\n";
		return 1;
	}
	ScalarConverter::convert(av[1]);
	return 0;
}