#include "Dog.hpp"

Dog::Dog(): Animal()
{
	this->type = "Dog";
	std::cout << "[Dog] Default constructor called\n";
}

Dog::Dog( const Dog& other ) : Animal(other)
{

}

Dog& Dog::operator=( const Dog& other)
{
	if (this != &other)
		Animal::operator=(other);
	return *this;
}

Dog::~Dog()
{
	std::cout << "[Dog] Destructor called\n";
}

void	Dog::makeSound() const
{
	std::cout << this->type << " Woof! Woof\n";
}