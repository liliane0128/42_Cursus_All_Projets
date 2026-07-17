#include "Array.hpp"
#include <iostream>

int main()
{
	std::cout << PINK << "=== Int Array Test\n\n" << RESET;

	Array<int> a(5);
	a[0] = 42;
	a[1] = 999;
	a[2] = 32;
	a[3] = -12;
	a[4] = 0;

	std::cout << "Created an int Array a: ";
	for (int i = 0 ; i < 5; i++)
		std::cout << a[i] << " ";
	
	std::cout << GREEN << "\n\n--- Copy Constructor Test ---\n" << RESET;
	Array<int> b(a);
	std::cout << "b(copied a): ";
	for (int i = 0 ; i < 5; i++)
		std::cout << b[i] << " ";

	std::cout << GREEN << "\n\n--- Copy Assignment Test ---\n" << RESET;

	Array<int> c(3);
	c[0] = 1;
	c[1] = 2;
	c[2] = 3;

	std::cout << "Original C: ";

	for (int i = 0 ; i < 3; i++)
		std::cout << c[i] << " ";
	std::cout << "\n";
	
	c = a;
	std::cout << "C after copy assignement to a: ";
	for (int i = 0 ; i < 5; i++)
		std::cout << c[i] << " ";

	std::cout << "\n";

	std::cout << GREEN << "\n--- Try to access an array out of range ---\n" << RESET;
	try
	{
		std::cout << a[6] << std::endl;

	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << '\n' << RESET;
	}

	std::cout << PINK << "\n=== String Array Test\n" << RESET;

	Array<std::string> s(3);
	s[0] = "hello";
	s[1] = " world";
	s[2] = "!";
	std::cout << s[0] << s[1] << s[2] << std::endl;

	std::cout << GREEN << "\n--- Try to access an array out of range ---\n" << RESET;
	try
	{
		std::cout << s[4];
	}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << '\n' << RESET;
	}
	
	
	return 0;
}