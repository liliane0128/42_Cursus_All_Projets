#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {}
ScalarConverter::~ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter& other)
{
	*this = other;
}
ScalarConverter& ScalarConverter::operator=(const ScalarConverter& other)
{
	(void)other;
	return *this;
}

/**
 * - nan / nanf: "Not-a-Number" results from undefined operations (e.g., 0/0).
 * - inf / +inf: Positive infinity, occurs when a positive value overflows 
 * the double/float range (e.g., 1.0/0.0).
 * - -inf / -inff: Negative infinity, occurs when a negative value overflows 
 * (e.g., -1.0/0.0).
 */
static bool	isSpecific(const std::string& input)
{
	if (input == "nan" || input == "nanf" || input == "inf"
		|| input == "+inf" || input == "+inff" || input == "inff"
		|| input == "-inf" || input == "-inff")
		return true;
	return false;
}

static void	printSpecific(const std::string& input)
{
	if (input == "nan" || input == "nanf")
	{
		std::cout << "char: impossible\nint: impossible\n"
				  << "float: nanf\ndouble: nan\n";
		return;
	}
	if (input == "+inf" || input == "+inff" || input == "inf" || input == "inff")
	{
		std::cout << "char: impossible\nint: impossible\n"
				  << "float: inff\ndouble: inf\n";
		return;
	}
	if (input == "-inf" || input == "-inff")
	{
		std::cout << "char: impossible\nint: impossible\n"
				  << "float: -inff\ndouble: -inf\n";	
	  	return;
	}
}

/**
 * @note Formatting Floating-Point Output:
 * * std::fixed: Forces the output to use fixed-point notation instead 
 * of scientific notation and ensures the decimal point is always shown.
 * * std::setprecision(1): When used with std::fixed, it determines the 
 * exact number of digits after the decimal point. 
 * * Together, they ensure that a value like 42 is printed as 42.0.
 */
static void	printFromChar(const std::string& input)
{
	char c = input[0];
	if (std::isprint(c))
		std::cout << "char: '" << c << "'" << std::endl;
	else
		std::cout << "char: Non displayable" << std::endl;

	std::cout << "int: " << static_cast<int>(c) << std::endl;

	std::cout << std::fixed << std::setprecision(1);
	std::cout << "float: " << static_cast<float>(c) << "f" << std::endl;
	std::cout << "double: " << static_cast<double>(c) << std::endl;
	return;
}

static void	printNumber(const double val)
{
	std::cout << "char: ";
	if (val < 0 || val > 127 || std::isnan(val))
		std::cout << "impossible" << std::endl;
	else if (!std::isprint(static_cast<int>(val)))
		std::cout << "Non displayable" << std::endl;
	else
		std::cout << "'" << static_cast<char>(val) << "'" << std::endl;
	std::cout << "int: ";
	if (val < INT_MIN || val > INT_MAX
		|| std::isnan(val))
		std::cout << "impossible" << std::endl;
	else
		std::cout << static_cast<int>(val) << std::endl;
	std::cout << std::fixed << std::setprecision(1);
	std::cout << "float: " << static_cast<float>(val) << "f" << std::endl;
	std::cout << "double: " << val << std::endl;
	return;
}
		
static void printAllImpossible()
{
	std::cout << "char: impossible\nint: impossible\n"
				  << "float: impossible\ndouble: impossible\n";
}

void	ScalarConverter::convert(const std::string& input)
{
	if (input.empty())
		return;

	if (isSpecific(input))
	{
		printSpecific(input);
		return ;
	}

	if (input.length() == 1 && !std::isdigit(input[0]))
	{
		printFromChar(input);
		return;
	}
	
	char* end;
	double val = std::strtod(input.c_str(), &end);
	bool isValid = (end != input.c_str()) 
		&& (*end == '\0' || (*end == 'f' && *(end + 1) == '\0'));
	
	if (isValid)
		printNumber(val);
	else
		printAllImpossible();
}
