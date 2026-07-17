#include "Bureaucrat.hpp"

Bureaucrat::Bureaucrat(): _name("Default Bureaucrat"), _grade(150) {}

Bureaucrat::Bureaucrat(std::string name, int grade):
_name(name)
{
	if (grade < 1)
		throw Bureaucrat::GradeTooHighException();
	if (grade > 150)
		throw Bureaucrat::GradeTooLowException();
	_grade = grade;
}


Bureaucrat::~Bureaucrat() {}

Bureaucrat::Bureaucrat(const Bureaucrat& other): 
_name(other._name), _grade(other._grade)
{

}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other)
{
	if (this != &other )
		this->_grade = other.getGrade();
	return *this;
}


std::string	Bureaucrat::getName() const
{
	return this->_name;
}

int	Bureaucrat::getGrade() const
{
	return this->_grade;
}


void	Bureaucrat::incrementGrade()
{
	if (_grade <= 1)
		throw Bureaucrat::GradeTooHighException();
	_grade--;
}

void	Bureaucrat::decrementGrade()
{
	if (_grade >= 150)
		throw Bureaucrat::GradeTooLowException();
	_grade++;
}

std::ostream& operator<<(std::ostream& output, const Bureaucrat& b)
{
	output << b.getName() << ", bureaucrat grade " << b.getGrade() << ".";
	return output;
}

