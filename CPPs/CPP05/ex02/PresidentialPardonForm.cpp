#include "PresidentialPardonForm.hpp"

PresidentialPardonForm::PresidentialPardonForm():
AForm("Presidential Pardon", 25, 5), _target("default") {}

PresidentialPardonForm::PresidentialPardonForm(std::string target):
AForm("Presidential Pardon", 25, 5), _target(target) {}

PresidentialPardonForm::~PresidentialPardonForm() {}

PresidentialPardonForm::PresidentialPardonForm(const PresidentialPardonForm& other):
AForm(other), _target(other._target)
{

}

PresidentialPardonForm& PresidentialPardonForm::operator=(const PresidentialPardonForm& other)
{
	if (this != &other)
		AForm::operator=(other);
	this->_target = other._target;

	return *this;
}

void	PresidentialPardonForm::execute(Bureaucrat const& executor) const
{
	this->checkRequirements(executor);

	std::cout << this->_target << " has been pardoned by Zaphod Beeblebrox.\n";
}
