#include "RobotomyRequestForm.hpp"

RobotomyRequestForm::RobotomyRequestForm(std::string target):
AForm("Robotomy Request", 72, 45),
_target(target)
{

}

RobotomyRequestForm::~RobotomyRequestForm() {}

RobotomyRequestForm::RobotomyRequestForm(const RobotomyRequestForm& other):
AForm(other), _target(other._target)
{}

RobotomyRequestForm& RobotomyRequestForm::operator=(const RobotomyRequestForm& other)
{
	if (this != &other)
		AForm::operator=(other);
	this->_target = other._target;

	return *this;
}

void	RobotomyRequestForm::execute(Bureaucrat const& executor) const
{
	this->checkRequirements(executor);

	std::cout << "* VRRRRRRRRRRRR... *\n";

	if (std::rand() % 2)
		std::cout << this->_target 
		<< " has been robotomized successfully.\n";
	else
		std::cout << "The robotomy on " << this->_target
		<< " failed.\n";
}