#include "Brain.hpp"

Brain::Brain() : ideas()
{
	std::cout << "[Brain] Default constructor called\n";
}

Brain::~Brain()
{
	std::cout << "[Brain] Destructor called\n";
}

Brain::Brain( const Brain& other )
{
	std::cout << "[Brain] Copy constructor called\n";

	for (int i = 0; i < 100; i++)
		this->ideas[i] = other.ideas[i];
}

Brain& Brain::operator=( const Brain& other )
{
	std::cout << "[Brain] Assignment operator called\n";
	
	if (this != &other)
	{
		for (int i = 0; i < 100; i++)
			this->ideas[i] = other.ideas[i];
	}
	return *this;
}

void	Brain::setIdeas( int index, std::string idea )
{
	if (index >= 0 && index < 100)
		this->ideas[index] = idea;
}

std::string Brain::getIdeas( int index ) const
{
	if (index >= 0 && index < 100)
		return this->ideas[index];
	return "";
}
