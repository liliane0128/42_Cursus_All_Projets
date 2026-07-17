#ifndef AFORM_HPP
# define AFORM_HPP

#include <string>
#include <iostream>

class Bureaucrat;

class AForm
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

	class NotSignedException : public std::exception
	{
		public:
		virtual const char* what() const throw()
		{
			return "the form is not signed";
		}
	};

	AForm();
	AForm(std::string name, int gradeToSign, int gradeToExecute);
	virtual ~AForm();
	AForm(const AForm& other);
	AForm& operator=(const AForm& other);

	std::string	getName() const;
	bool		getIsSigned() const;
	int			getGradeToSign() const;
	int			getGradeToExecute() const;

	void		beSigned(const Bureaucrat& b);

	void		checkRequirements(const Bureaucrat& executor) const;
	virtual void execute(Bureaucrat const& executor) const = 0;
};

std::ostream& operator<<(std::ostream& output, const AForm& f);

#endif