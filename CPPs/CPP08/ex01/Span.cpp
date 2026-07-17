#include "Span.hpp"

Span::Span(): _n(0) {}

/**
 * * reserve(n): 
 * Pre-allocates memory for 'n' elements without changing the vector's size.
 * This prevents multiple expensive memory reallocations and data copies 
 * when adding elements, as the underlying array is guaranteed not to 
 * grow until the count exceeds 'n'.
 */
Span::Span(unsigned int n): _n(n)
{
	_elements.reserve(n);
}

Span::~Span() {}

Span::Span(const Span& other): _n(other._n), _elements(other._elements) {}

Span& Span::operator=(const Span& other)
{
	if (this != &other)
	{
		this->_n = other._n;
		this->_elements = other._elements;
	}
	return *this;
}


void	Span::addNumber(int value)
{
	if (_elements.size() >= _n)
		throw std::out_of_range("Span is full");
	_elements.push_back(value);
}

int		Span::longestSpan()
{
	if (_elements.size() <= 1)
		throw std::logic_error("Span: too few elements to calculate span");
	std::vector<int>::iterator itMin = std::min_element(_elements.begin(), _elements.end());
	std::vector<int>::iterator itMax = std::max_element(_elements.begin(), _elements.end());
	return *itMax - *itMin;
}

int		Span::shortestSpan()
{
	if (_elements.size() <= 1)
		throw std::logic_error("Span: too few elements to calculate span");

	std::vector<int> sortedVec = _elements;
	std::sort(sortedVec.begin(), sortedVec.end());

	int	minDiff = INT_MAX;
	for (size_t i = 0; i < sortedVec.size() - 1; i++)
	{
		int diff = sortedVec[i + 1] - sortedVec[i];
		if (diff < minDiff)
			minDiff = diff;
		if (minDiff == 0)	return 0;
	}

	return minDiff;
}

void	Span::display() const
{
	for (size_t i = 0; i < _elements.size(); i++)
		std::cout << _elements[i] << " ";
	std::cout << std::endl;
}


