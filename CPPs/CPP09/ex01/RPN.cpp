#include "RPN.hpp"

RPN::RPN() {}
RPN::~RPN() {}
RPN::RPN(const RPN& other): _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
		this->_stack = other._stack;
	return *this;
}

void	RPN::calculate(char op)
{
	if (_stack.size() < 2)
		throw std::runtime_error("Error: too few elements to calcaulate");
	long long right = _stack.top();
	_stack.pop();
	long long left = _stack.top();
	_stack.pop();

	long long res;
	switch (op)
	{
		case '+':
			res = left + right;
			break;
		case '-':
			res = left - right;
			break;
		case '/':
			if (right == 0)
				throw std::runtime_error("Error: Division by zero");
			res = left / right;
			break;
		case '*':
			res = left * right;
			break;
		default:
			throw std::runtime_error("Error: Invalid operator");
	}
	if (res > INT_MAX || res < INT_MIN)
			throw std::runtime_error("Error: Overflow");
	_stack.push(static_cast<int>(res));
}

void	RPN::execute(const std::string& input)
{
	while (!_stack.empty())
		_stack.pop();
	std::stringstream ss(input);
	std::string token;
	while (ss >> token)
	{
		if (token.length() != 1)
			throw std::runtime_error("Error: number must between 0-9");

		char c = token[0];
		if (std::isdigit(static_cast<unsigned char>(c)))
			_stack.push(c - '0');
		else if (c == '+' || c == '-' || c == '/' || c == '*')
			calculate(c);
		else
			throw std::runtime_error("Error: must be a number or an operator");
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: Invalid RPN expression");

	std::cout << _stack.top() << std::endl;
}