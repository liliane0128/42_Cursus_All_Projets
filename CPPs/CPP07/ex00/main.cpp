#include "functions.hpp"
#include <string>
#include <iostream>

int main()
{
	std::string s1 = "hello";
	std::string s2 = "world";
	int a = 42;
	int b = 999999;
	int c = 999999;

	swap(s1, s2);
	std::cout << s1 << " " <<  s2 << "\n";
	swap(a, b);
	std::cout << "a: " << a << " b:" << b << "\n";
	int min = ::min(a, b);
	int max = ::max(a, b);
	std::cout << "Min: " << min << " Max: " << max << "\n";

	std::cout << "\nIf two values are equal: ";
	std::cout << ::min(a, c) << " " << ::max(a, c) << "\n";

	std::string minS = ::min(s1, s2);
	std::string maxS = ::max(s1, s2);
	std::cout << "Min: " << minS << " Max: " << maxS << "\n";

	return 0;
}

