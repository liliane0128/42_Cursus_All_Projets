#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	std::cout << "===Basic Test===\n\n";
	
	Bureaucrat boss("boss", 20);
	Form f1("f1", 20, 20);
	Form f2(f1);
	Form f3("f3", 150, 130);
	f1.beSigned(boss);
	boss.signForm(f2);
	f3 = f1;
	std::cout << boss << "\n" << f1 << "\n" << f2 << "\n" << f3 << "\n";

	Form f4("f4", 10, 10);
	boss.signForm(f4);

	

	std::cout << "\n===Exception Test===\n\n";
	try
	{
		Bureaucrat b("b", 30);
		f2.beSigned(b);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	try
	{
		Form f4("f4", 151, 151);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << "\n";
	}

	try
	{
		Form f5("f5", 0, 20);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

	return 0;
}