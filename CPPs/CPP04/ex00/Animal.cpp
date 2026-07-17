#include "Animal.hpp"

Animal::Animal() : type("General Animal")
{
	std::cout << "[Animal] Default constructor called\n";
}

Animal::~Animal()
{
	std::cout << "[Animal] Destructor called\n";
}

//Use Initialization List for a better performance
Animal::Animal( const Animal&  other ) : type(other.type)
{

}

Animal& Animal::operator=( const Animal& other )
{
	if (this != &other) //Comparing two addresses to avoid self-assignement
		this->type = other.type;

	return *this;
}

void	Animal::makeSound() const
{
	std::cout << this->type << " makes a noise\n";
}

std::string Animal::getType() const
{
	return (this->type);
}
