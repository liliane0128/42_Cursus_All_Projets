#include "Cat.hpp"

Cat::Cat(): Animal()
{
	this->brain = new Brain();
	this->type = "Cat";
	std::cout << "[Cat] Default constructor called\n";
}

Cat::Cat( const Cat& other ) : Animal(other)
{
	if (other.brain)
		this->brain = new Brain(*other.brain);
	else
		this->brain = NULL;
}

Cat& Cat::operator=( const Cat& other)
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

Cat::~Cat()
{
	delete this->brain;
	std::cout << "[Cat] Destructor called\n";
}

void	Cat::makeSound() const
{
	std::cout << this->type << " Meow! Meow!\n";
}

Brain*	Cat::getBrain() const
{
	return this->brain;
}
