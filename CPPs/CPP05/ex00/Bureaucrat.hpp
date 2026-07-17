#ifndef BUREAUCRAT_HPP
# define BUREAUCRAT_HPP

#include <string>
#include <iostream>

/**
 * @class ExceptionClass
 * @brief Represents a custom runtime error condition.
 * * This class follows the standard C++ exception pattern by inheriting from 
 * std::exception. It overrides the what() method to provide a descriptive 
 * error message when the exception is caught, ensuring type-safe and 
 * structured error handling throughout the application.
 */

class Bureaucrat
{
private:
	std::string const _name;
	int	_grade;

public:
	class GradeTooHighException : public std::exception
	{
		public:
		virtual const char* what() const throw()
		{
			return "Grade is too high (above 1)!";
		}
	};

	class GradeTooLowException : public std::exception
	{
		public:
		virtual const char* what() const throw()
		{
			return "Grade is too low (below 150)!";
		}
	};

	Bureaucrat();
	Bureaucrat(std::string name, int grade);
	~Bureaucrat();
	Bureaucrat(const Bureaucrat& other);
	Bureaucrat& operator=(const Bureaucrat& other);

	std::string	getName() const;
	int			getGrade() const;

	void		incrementGrade();
	void		decrementGrade();
};

std::ostream& operator<<(std::ostream& output, const Bureaucrat& b);

#endif
