#include "Dog.hpp"

Dog::Dog(): Animal()
{
	this->brain = new Brain();
	this->type = "Dog";
	std::cout << "[Dog] Default constructor called\n";
}


Dog::Dog( const Dog& other ) : Animal(other)
{
	if (other.brain)
		this->brain = new Brain(*other.brain);
	else
		this->brain = NULL;
}

Dog& Dog::operator=( const Dog& other )
{
	if (this != &other)
	{
		Animal::operator=(other);
		Brain* tmp = new Brain(*other.brain);
		delete this->brain;
		this->brain = tmp;
	}
	return *this;
}

Dog::~Dog()
{
	delete this->brain;
	std::cout << "[Dog] Destructor called\n";
}

void	Dog::makeSound() const
{
	std::cout << this->type << " Woof! Woof!\n";
}

Brain* Dog::getBrain() const
{
	return this->brain;
}