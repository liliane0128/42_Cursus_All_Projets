#include "MutantStack.hpp"
#include <list>
#include <deque>
#include <vector>

int main()
{
	MutantStack<int> mstack;

	mstack.push(5);
	mstack.push(17);

	std::cout << mstack.top() << std::endl;

	mstack.pop();
	std::cout << mstack.size() << std::endl;

	mstack.push(3);
	mstack.push(5);
	mstack.push(737);

	mstack.push(0);

	mstack.display();
	MutantStack<int>::iterator it = mstack.begin(); //5
	MutantStack<int>::iterator ite = mstack.end(); 

	++it;
	--it;
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}
	std::stack<int> s(mstack);

	MutantStack<std::string> stringStack;
	stringStack.push("hello");
	stringStack.push("world");

	std::cout << stringStack.top() << std::endl;
	stringStack.pop();
	std::cout << stringStack.top() << std::endl;

	stringStack.push("world");
	stringStack.push("how");
	stringStack.push("are");
	stringStack.push("you");



	for (MutantStack<std::string>::iterator it1 = stringStack.begin(); it1 != stringStack.end(); ++it1)
		std::cout << *it1 << " ";
	std::cout << std::endl;
	//stringStack.display();

	MutantStack<std::string>::iterator it2 = stringStack.begin();
	std::cout << *(it2 + 3) << std::endl;

	{
		std::cout << "\n--Compare with other containers---\n";
		MutantStack<int> testStack;
		testStack.push(2);
		testStack.push(4);
		testStack.push(6);
		testStack.pop();
		testStack.push(8);

		testStack.display();

		std::list<int> testList;
		testList.push_back(2);
		testList.push_back(4);
		testList.push_back(6);
		testList.pop_back();
		testList.push_back(8);

		for (std::list<int>::iterator it = testList.begin(); it != testList.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;

		std::deque<int> testDeque;
		testDeque.push_back(2);
		testDeque.push_back(4);
		testDeque.push_back(6);
		testDeque.pop_back();
		testDeque.push_back(8);
		for (std::deque<int>::iterator it1 = testDeque.begin(); it1 != testDeque.end(); ++it1)
			std::cout << *it1 << " ";
		std::cout << std::endl;

		std::vector<int> testV;
		testV.push_back(2);
		testV.push_back(4);
		testV.push_back(6);
		testV.pop_back();
		testV.push_back(8);
		for (std::vector<int>::iterator it2 = testV.begin(); it2 != testV.end(); ++it2)
			std::cout << *it2 << " ";
		std::cout << std::endl;

	}

	return 0;
}
