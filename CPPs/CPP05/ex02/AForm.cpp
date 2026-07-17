#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm(): _name("Default AForm"), _isSigned(false), _gradeToSign(150), _gradeToExecute(150) {}

AForm::AForm(std::string name, int gradeToSign, int gradeToExecute):
_name(name), _isSigned(false),
_gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (this->_gradeToSign < 1 || this->_gradeToExecute < 1)
		throw AForm::GradeTooHighException();
	else if (this->_gradeToSign > 150 || this->_gradeToExecute > 150)
		throw AForm::GradeTooLowException();
}

AForm::~AForm() {}

AForm::AForm(const AForm& other):
_name(other._name), _isSigned(other._isSigned),
 _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute) {}

AForm& AForm::operator=(const AForm& other)
{
	if (this != &other)
		this->_isSigned = other.getIsSigned();
	return *this;
}

std::string	AForm::getName() const
{
	return this->_name;
}

bool	AForm::getIsSigned() const
{
	return this->_isSigned;
}

int	AForm::getGradeToSign() const
{
	return this->_gradeToSign;
}

int	AForm::getGradeToExecute() const
{
	return this->_gradeToExecute;
}

void	AForm::beSigned(const Bureaucrat& b)
{
	if (this->_isSigned == true)
	{
		std::cerr << "The AForm " << this->getName() << " is already signed\n";
		return;
	}

	if (b.getGrade() > this->getGradeToSign())
		throw AForm::GradeTooLowException();

	this->_isSigned = true;
}


std::ostream& operator<<(std::ostream& output, const AForm& f)
{
	output << "AForm: " << f.getName()
		   << ", signed: " << (f.getIsSigned() ? "yes" : "no")
		   << ", grade to sign: " << f.getGradeToSign()
		   << ", grade to execute: " << f.getGradeToExecute();
	
	return output;
}


void	AForm::checkRequirements(const Bureaucrat& executor) const
{
	if (!this->getIsSigned())
		throw AForm::NotSignedException();
	if (executor.getGrade() > this->getGradeToExecute())
		throw AForm::GradeTooLowException();
}

