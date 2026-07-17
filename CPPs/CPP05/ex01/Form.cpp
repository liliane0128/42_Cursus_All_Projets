#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form(): _name("Default Form"), _isSigned(false), _gradeToSign(150), _gradeToExecute(150) {}

Form::Form(std::string name, int gradeToSign, int gradeToExecute):
_name(name), _isSigned(false),
_gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (this->_gradeToSign < 1 || this->_gradeToExecute < 1)
		throw Form::GradeTooHighException();
	else if (this->_gradeToSign > 150 || this->_gradeToExecute > 150)
		throw Form::GradeTooLowException();
}

Form::~Form() {}

Form::Form(const Form& other):
_name(other._name), _isSigned(other._isSigned),
 _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute) {}

Form& Form::operator=(const Form& other)
{
	if (this != &other)
		this->_isSigned = other.getIsSigned();
	return *this;
}

std::string	Form::getName() const
{
	return this->_name;
}

bool	Form::getIsSigned() const
{
	return this->_isSigned;
}

int	Form::getGradeToSign() const
{
	return this->_gradeToSign;
}

int	Form::getGradeToExecute() const
{
	return this->_gradeToExecute;
}

void	Form::beSigned(const Bureaucrat& b)
{
	if (this->_isSigned == true)
	{
		std::cout << "The Form " << this->getName() << " is already signed\n";
		return;
	}

	if (b.getGrade() > this->getGradeToSign())
		throw Form::GradeTooLowException();

	this->_isSigned = true;
}


std::ostream& operator<<(std::ostream& output, const Form& f)
{
	output << "Form: " << f.getName()
		   << ", signed: " << (f.getIsSigned() ? "yes" : "no")
		   << ", grade to sign: " << f.getGradeToSign()
		   << ", grade to execute: " << f.getGradeToExecute();
	
	return output;
}
