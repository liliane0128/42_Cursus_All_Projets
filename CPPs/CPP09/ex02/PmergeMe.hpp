#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <sstream> 
#include <iterator>
#include <vector>
#include <deque>
#include <list>
#include <algorithm> // std::lower_bound
#include <ctime> //std::clock()
#include <iomanip> // std::fixed, std::setprecision
#include <climits> //INT_MAX


class PmergeMe
{
private:
	std::vector<int> vec;
	std::deque<int> deq;
	std::list<int> list;

	std::vector<int> generateJacobsthal(int n);
	void	display() const;
	void	sortVector(std::vector<int>& arr);
	void	sortDeque(std::deque<int>& arr);
	void	sortList(std::list<int>& arr);

	template <typename Container>
	bool isSorted(const Container& c)
	{
		typename Container::const_iterator it = c.begin();
		typename Container::const_iterator next = it;

		if (c.size() <= 1)
			return true;
		next++;
		while (next != c.end())
		{
			if (*it > *next)
				return false;
			it++;
			next++;
		}
		return true;
	}

public:
	PmergeMe();
	~PmergeMe();
	PmergeMe(const PmergeMe& other);
	PmergeMe& operator=(const PmergeMe& other);

	void	parseInput(int ac, char **av);
	void	execute();
};

#endif
