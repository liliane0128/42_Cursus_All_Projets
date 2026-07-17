#include "easyfind.hpp"
#include <list>
#include <deque>
#include <vector>
#include <iostream>
#include <string>

int main()
{
	std::vector<int> emptyList;
	try
	{
		std::vector<int>::iterator t = easyfind(emptyList, 42);
		std::cout << *t << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	
	std::list<int> firstList;
	firstList.push_back(10);
	firstList.push_back(5);

	std::list<int>::iterator it = easyfind(firstList, 10);
	std::cout << *it << std::endl;
	try
	{
		std::list<int>::iterator itt = easyfind(firstList, 2);
		std::cout << *itt << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	std::vector<int> secondList;
	secondList.push_back(4222222);
	secondList.push_back(18);
	std::vector<int>::iterator it1 = easyfind(secondList, 18);
	std::cout << *it1 << std::endl;

	std::deque<int> thirdList;
	thirdList.push_front(33);
	thirdList.push_front(22);
	thirdList.push_back(11);
	std::deque<int>::iterator it2 = easyfind(thirdList, 22);
	std::cout << *it2 << std::endl;
	

	return 0;
}