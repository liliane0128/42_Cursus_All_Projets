#include "ShrubberyCreationForm.hpp"

ShrubberyCreationForm::ShrubberyCreationForm(): 
AForm("Shrubbery Creation", 145, 137),_target("default") {}

ShrubberyCreationForm::ShrubberyCreationForm(std::string target): 
AForm("Shrubbery Creation", 145, 137), _target(target) {}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm& other):
AForm(other), _target(other._target)
{
}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(const ShrubberyCreationForm& other)
{
	if (this != &other)
		AForm::operator=(other);
	this->_target = other._target;
	return *this;
}

void	ShrubberyCreationForm::execute(Bureaucrat const& executor) const
{
	this->checkRequirements(executor);

	std::string fileName = this->_target + "_shrubbery";
	std::ofstream outfile(fileName.c_str());

	if (!outfile.is_open())
	{
		std::cerr << "open file failed\n";
		return;
	}

	outfile << "     ^     " << std::endl;
	outfile << "    / \\  	" << std::endl;
	outfile << "   /   \\  " << std::endl;
	outfile << "  /     \\  " << std::endl;
	outfile << " /_______\\ " << std::endl;
	outfile << "   |   |    " << std::endl;
}
