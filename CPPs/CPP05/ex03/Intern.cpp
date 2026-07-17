#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern() {}

Intern::~Intern() {}

Intern::Intern(const Intern &other)
{
	*this = other;
}

Intern &Intern::operator=(const Intern &other)
{
	(void)other;
	return *this;
}

AForm *Intern::makeForm(std::string name, std::string target)
{
	std::string names[] =
		{
			"Shurubbery Creation",
			"Robotomy Request",
			"Presidential Pardon"};

	int i = 0;
	while (i < 3 && names[i] != name)
		i++;
	AForm *res = NULL;
	switch (i)
	{
	case 0:
		res = new ShrubberyCreationForm(target);
		break;
	case 1:
		res = new RobotomyRequestForm(target);
		break;
	case 2:
		res = new PresidentialPardonForm(target);
		break;
	default:
		std::cout << "Error: Intern cannot create " << name
				  << " because it doesn't exist.\n";
		return NULL;
	}
	std::cout << "Intern creates " << name << std::endl;
	return res;
}