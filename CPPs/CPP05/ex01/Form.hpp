#ifndef FORM_HPP
# define FORM_HPP

#include <string>
#include <iostream>

class Bureaucrat;

class Form
{
private:
	std::string const _name;
	bool			_isSigned;
	int	const	_gradeToSign;
	int	const	_gradeToExecute;

public:
	class GradeTooHighException : public std::exception
	{
		public:
		virtual const char* what() const throw()
		{
			return "Grade too high!";
		}
	};

	class GradeTooLowException : public std::exception
	{
		public:
		virtual const char* what() const throw()
		{
			return "Grade too low!";
		}
	};

	Form();
	Form(std::string name, int gradeToSign, int gradeToExecute);
	~Form();
	Form(const Form& other);
	Form& operator=(const Form& other);

	std::string	getName() const;
	bool		getIsSigned() const;
	int			getGradeToSign() const;
	int			getGradeToExecute() const;

	void		beSigned(const Bureaucrat& b);
};

std::ostream& operator<<(std::ostream& output, const Form& f);

#endif