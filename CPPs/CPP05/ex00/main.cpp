#include "Bureaucrat.hpp"

int main()
{
	std::cout << "===Basic Test===\n\n";
	
	Bureaucrat d;	//default
	Bureaucrat a("A", 10);
	Bureaucrat b(a); 
	Bureaucrat c("C", 150);
	c = a; //the garde change to A's, name stay the same cause name is const

	std::cout << d << std::endl;
	std::cout << a << "\n" << b << "\n" << c << "\n";

	a.incrementGrade();
	c.decrementGrade();

	std::cout << "After in/decrement:\n" << a << "\n" << c << "\n";

	std::cout << "\n===Try to create an object out of range===\n\n";
	try
	{
		Bureaucrat error("error", 151);
		std::cout << "Success\n"; //will not print
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	try
	{
		Bureaucrat exception("exception", 0);
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	std::cout << "\n===Increment/Decrement out of range Test===\n\n";

	try
	{
		Bureaucrat f("f", 1);
		std::cout << f << "\n";
		f.incrementGrade();
	}
	catch(std::exception& e)
	{
		std::cerr << "Increment test out of range: "<< e.what() << '\n';
	}
	
	try
	{
		Bureaucrat f("f", 150);
		std::cout << f << "\n";
		f.decrementGrade();
	}
	catch(std::exception& e)
	{
		std::cerr << "Decrement test out of range: "<< e.what() << '\n';
	}
	
	return 0;
}