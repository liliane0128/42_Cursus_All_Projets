#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "Intern.hpp"


int main()
{
	Intern intern;
	AForm* random;
	Bureaucrat boss("boss", 1);
	Bureaucrat dir("director", 140);

	random = intern.makeForm("Robotomy Request", "Bender");
	std::cout << *random << "\n";

	random->beSigned(boss);
	random->execute(boss);

	std::cout << std::endl;

	AForm *s = intern.makeForm("Shurubbery Creation", "Shu");
	std::cout << *s << "\n";
	try
	{
		s->execute(dir);
	}
	catch(const std::exception& e)
	{
		std::cerr << dir.getName() << " can't execute " << s->getName() << " because " << e.what() << '\n';
	}
	s->beSigned(dir);
	s->execute(boss);
	

	std::cout << std::endl;

	AForm* r = intern.makeForm("Presidential Pardon", "President");
	std::cout << *r << "\n";
	r->beSigned(boss);

	try
	{
		r->execute(dir);
	}
	catch(const std::exception& e)
	{
		std::cerr << dir.getName() << " can't execute " << r->getName() << " because " << e.what() << '\n';
	}
	r->execute(boss);
	

	std::cout << std::endl;

	
	AForm* unknown = intern.makeForm("make coffee", "CC");
	if (unknown == NULL)
		std::cout << "is null\n";
	
	delete random;
	delete r;
	delete s;
	delete unknown;

	return 0;
}