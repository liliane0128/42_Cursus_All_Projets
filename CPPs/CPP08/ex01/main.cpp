#include "Span.hpp"
#include <cstdlib>
#include <list>
#include <deque>
#include <ctime>

int main()
{
	std::srand(std::time(NULL));
	Span sp = Span(10);
	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	sp.display();

	std::cout << sp.shortestSpan() << std::endl;
	std::cout << sp.longestSpan() << std::endl;


	std::vector<int> list;
	list.push_back(199);
	list.push_back(3);
	sp.addRange(list.begin(), list.end());
	sp.display();

	
	std::cout << sp.shortestSpan() << std::endl;
	std::cout << sp.longestSpan() << std::endl;

	std::vector<int> tmp;
	for (int i = 0; i < 10000; i++)
		tmp.push_back(rand());

	Span vec = Span(10000);
	vec.addRange(tmp.begin(), tmp.end());

	std::cout << "\n" << vec.shortestSpan() << std::endl;
	std::cout << vec.longestSpan() << std::endl;

	return 0;
}