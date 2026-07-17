#include "Animal.hpp"

Animal::Animal() : type("General Animal")
{
	std::cout << "[Animal] Default constructor called\n";
}

Animal::~Animal()
{
	std::cout << "[Animal] Destructor called\n";
}

Animal::Animal( const Animal&  other ) : type(other.type)
{

}

Animal& Animal::operator=( const Animal& other )
{
	if (this != &other)
		this->type = other.type;

	return *this;
}

std::string Animal::getType() const
{
	return (this->type);
}
