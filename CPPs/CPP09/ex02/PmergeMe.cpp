#include "PmergeMe.hpp"

PmergeMe::PmergeMe() : vec(), deq() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other)
{
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other) 
	{
		this->vec = other.vec;
		this->deq = other.deq;
	}

	return *this;
}

/*
* Jacobsthal sequence improves insertion efficiency by reducing unnecessary structural 
* shifts in the container. It spreads insertions in a non-linear order, which lowers average
* insertion cost during the merge phase.
*/
std::vector<int> PmergeMe::generateJacobsthal(int n)
{
	std::vector<int> insertOrder;

	if (n <= 0)	return insertOrder;
	if (n < 3) {
		for (int i = 1; i <= n; ++i)
			insertOrder.push_back(i);
		return insertOrder;
	}

	std::vector<int> jacob;
	jacob.push_back(3);
	jacob.push_back(5);
	for (int i = 2; i < 15; i++)
		jacob.push_back(jacob.at(i-1) + (2 * (jacob.at(i-2))));

	int lastJacob = 1;

	for (size_t i = 0; i < jacob.size(); ++i)
    {
		int currentJacob = jacob[i];
        if (lastJacob >= n) break;

        int start = (currentJacob > n) ? n : currentJacob;
        
        for (int k = start; k > lastJacob; --k) {
            insertOrder.push_back(k);
        }
        
        lastJacob = start;
    }
	return insertOrder;
}

void	PmergeMe::display() const 
{
	for (std::vector<int>::const_iterator it = this->vec.begin(); it != vec.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}

void	PmergeMe::sortVector(std::vector<int>& arr)
{
    if (arr.size() <= 1)
        return;

    std::vector<int> winners;
    std::vector<int> losers;

	for (size_t i = 0; i < arr.size() -1; i += 2)
    {
        if (arr[i] > arr[i + 1])
        {
            winners.push_back(arr[i]);
            losers.push_back(arr[i + 1]);
        }
        else
        {
            winners.push_back(arr[i + 1]);
            losers.push_back(arr[i]);
        }
    }

    bool hasStraggler = (arr.size() % 2 != 0);
    int straggler;
    if (hasStraggler)
        straggler = arr.back();

    sortVector(winners);
 
    std::vector<int> mainChain;
    for (size_t j = 0; j < winners.size(); j++)
        mainChain.push_back(winners[j]);

    std::vector<int> pending = losers;
    if (hasStraggler) {
	    pending.push_back(straggler);}

	std::vector<int> order = generateJacobsthal(pending.size());

	for (size_t j = 0; j < order.size(); j++)
	{
		int idx = order[j] - 1;
		if (idx < 0 || idx >= (int)pending.size()) {
	  		continue;}

		int value = pending[idx];

		std::vector<int>::iterator it =
			std::lower_bound(mainChain.begin(), mainChain.end(), value);
		mainChain.insert(it, value);
	}
    arr = std::vector<int>(mainChain.begin(), mainChain.end());
}

void	PmergeMe::sortDeque(std::deque<int>& arr)
{
    if (arr.size() <= 1)
        return;

    std::deque<int> winners;
    std::deque<int> losers;

	for (size_t i = 0; i < arr.size() -1; i += 2)
    {
        if (arr[i] > arr[i + 1])
        {
            winners.push_back(arr[i]);
            losers.push_back(arr[i + 1]);
        }
        else
        {
            winners.push_back(arr[i + 1]);
            losers.push_back(arr[i]);
        }
    }

    bool hasStraggler = (arr.size() % 2 != 0);
    int straggler = 0;
    if (hasStraggler)
        straggler = arr.back();

    sortDeque(winners);
 
    std::deque<int> mainChain;
    for (size_t j = 0; j < winners.size(); j++)
        mainChain.push_back(winners[j]);

    std::deque<int> pending = losers;
    if (hasStraggler) {
	    pending.push_back(straggler);}

	std::vector<int> order = generateJacobsthal(pending.size());
	for (size_t j = 0; j < order.size(); j++)
	{
		int idx = order[j] - 1;
		if (idx < 0 || idx >= (int)pending.size()) {
	  		continue;}
		int value = pending[idx];
		std::deque<int>::iterator it =
			std::lower_bound(mainChain.begin(), mainChain.end(), value);
		mainChain.insert(it, value);
	}
    arr = std::deque<int>(mainChain.begin(), mainChain.end());
}

void PmergeMe::sortList(std::list<int>& arr)
{
    if (arr.size() <= 1)
        return;

    std::list<int> winners;
    std::list<int> losers;

    std::list<int>::iterator it = arr.begin();
    while (it != arr.end())
    {
        int first = *it;
        ++it;

        if (it == arr.end())
        {
            winners.push_back(first);
            break;
        }

        int second = *it;
        if (first > second)
        {
            winners.push_back(first);
            losers.push_back(second);
        }
        else
        {
            winners.push_back(second);
            losers.push_back(first);
        }
        ++it;
    }

    bool hasStraggler = (arr.size() % 2 != 0);
    int straggler = 0;
    if (hasStraggler)
        straggler = arr.back();

    sortList(winners);

    std::list<int> mainChain;
    for (std::list<int>::iterator w_it = winners.begin();
         w_it != winners.end();
         ++w_it)
    {
        mainChain.push_back(*w_it);
    }

    std::list<int> pending = losers;
    if (hasStraggler)
        pending.push_back(straggler);

    std::vector<int> order = generateJacobsthal(pending.size());
    for (size_t j = 0; j < order.size(); j++)
    {
        int idx = order[j] - 1;

        if (idx < 0 || idx >= (int)pending.size())
            continue;

        std::list<int>::iterator p_it = pending.begin();
        std::advance(p_it, idx); //std::advance(it, n) moves the iterator it forward by n positions

        int value = *p_it;

        std::list<int>::iterator insert_it = mainChain.begin();
		while (insert_it != mainChain.end() && *insert_it < value)
			++insert_it;
        mainChain.insert(insert_it, value);
    }
    arr = mainChain;
}

void	PmergeMe::parseInput(int ac, char **av)
{
	if (ac < 2)
		throw std::runtime_error("Error: Not Enough Numbers");

	for (int i = 1; i < ac; i++)
	{
		if (std::string(av[i]).empty())
			throw std::runtime_error("Error: Empty Argument");

		std::stringstream ss(av[i]);
		long long n;
		while (ss >> n)
		{
			if (n < 0)
				throw std::runtime_error("Error: Negative Number");
			if (n > INT_MAX)
				throw std::runtime_error("Error: Number Too Large");
			vec.push_back(static_cast<int>(n));
			deq.push_back(static_cast<int>(n));
			list.push_back(static_cast<int>(n));

		}
		if (!ss.eof() && ss.fail())
			throw std::runtime_error("Error: Invalid Character");
	}
}

void	PmergeMe::execute()
{
	std::cout << "Before: ";
	this->display();

	std::vector<int> vec_copy = vec;
	std::deque<int> deq_copy = deq;
	std::list<int> list_copy = list;


	//clock() returns the processor time used by the program so far
	//time unit: clock ticks.  seconds = clock() / CLOCK_PER_SEC;
	clock_t start_vec = clock();
	this->sortVector(vec_copy);
	if (!isSorted(vec_copy)) 
		throw std::runtime_error("Vector is not sorted");
	clock_t end_vec = clock();
	double time_vec = static_cast<double>(end_vec - start_vec) * 1000000.0 / CLOCKS_PER_SEC;  //to get the microseconds

	clock_t start_deq = clock();
	this->sortDeque(deq_copy);
		if (!isSorted(deq_copy))
		throw std::runtime_error("Deque is not sorted");
	clock_t end_deq = clock();
	double time_deq = static_cast<double>(end_deq - start_deq) * 1000000.0 / CLOCKS_PER_SEC;

	clock_t start_list = clock();
	this->sortList(list_copy);
	if (!isSorted(list_copy))
		throw std::runtime_error("List is not sorted");
	clock_t end_list = clock();
	double time_list = static_cast<double>(end_list - start_list) * 1000000.0 / CLOCKS_PER_SEC;

	this->vec = vec_copy;
	this->deq = deq_copy;
	this->list = list_copy;

	std::cout << "After: ";
	this->display();

	std::cout << "Time to process a range of "  << this->vec.size() << " elements with std::vector : "
			  << std::fixed << std::setprecision(5)
			  << time_vec << " us" << std::endl;
	std::cout << "Time to process a range of "  << this->deq.size() << " elements with std::deque : "
			  << std::fixed << std::setprecision(5)
			  << time_deq << " us" << std::endl;
	std::cout << "Time to process a range of "  << this->list.size() << " elements with std::list : "
			  << std::fixed << std::setprecision(5)
			  << time_list << " us" << std::endl;
}
