#ifndef RPN_HPP
# define RPN_HPP

#include <stack>
#include <sstream> //std::stringstream 
#include <iostream>
#include <string>
#include <climits> //INT_MAX
#include <list>

class RPN
{
private:
	std::stack<int> _stack;

	void	calculate(char op);

public:
	RPN();
	~RPN();
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);

	void	execute(const std::string& input);
};

#endif