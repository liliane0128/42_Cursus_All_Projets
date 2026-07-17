#ifndef SPAN_HPP
# define SPAN_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>


class Span
{
private:
	unsigned int		_n;
	std::vector<int>	_elements;

public:
	Span();
	Span(unsigned int n);
	~Span();
	Span(const Span& other);
	Span& operator=(const Span& other);

	void	addNumber(int value);
	int		shortestSpan();
	int		longestSpan();

	void	display() const;
		
/*
** Template implementation must be visible to the compiler in the header.(.hpp/.tpp)
** Unlike regular functions, templates are "blueprints" and cannot be
** compiled into a .o file without knowing the specific types used in main().
*/
	template <typename Iterator>
	void addRange(Iterator begin, Iterator end)
	{
		int dist = std::distance(begin, end);
		if (dist + _elements.size() > _n)
			throw std::out_of_range("Span does not have enough space");
		_elements.insert(_elements.end(), begin, end);
	}
};


# endif
