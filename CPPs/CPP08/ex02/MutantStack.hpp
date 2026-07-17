#ifndef MUTANTSTACK_HPP
# define MUTANTSTACK_HPP

#include <iostream>
#include <stack>

/* 
 * * std::stack is a container adapter providing LIFO (Last-In, First-Out) logic.
 * It wraps an underlying container (defaulting to std::deque), exposing only a 
 * specific subset of methods (push, pop, and top) to ensure strict stack behavior.
 * By hiding direct access to internal elements and iterators, it guarantees that
 * data is processed in a predictable, disciplined order.
 */
template <typename T>
class MutantStack : public std::stack<T>
{
public:
	MutantStack() {}
	~MutantStack() {}
	MutantStack(const MutantStack& other): std::stack<T>(other) {}
	MutantStack& operator=(const MutantStack& other)
	{
		if (this != &other)
			std::stack<T>::operator=(other);
		return *this;
	}

	typedef typename std::stack<T>::container_type::iterator iterator;
	iterator begin() {return this->c.begin();}
	iterator end() {return this->c.end();}

	typedef typename std::stack<T>::container_type::const_iterator const_iterator;
	const_iterator begin() const {return this->c.begin();}
	const_iterator end() const {return this->c.end();}

	typedef typename std::stack<T>::container_type::reverse_iterator reverse_iterator;
	reverse_iterator rbegin() {return this->c.rbegin();}
	reverse_iterator rend() {return this->c.rend();}

	typedef typename std::stack<T>::container_type::const_reverse_iterator const_reverse_iterator;
	const_reverse_iterator rbegin() const {return this->c.rbegin();}
	const_reverse_iterator rend() const {return this->c.rend();}

	void	display() const
	{
		for (const_iterator it = this->begin(); it != this->end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}
};

#endif
