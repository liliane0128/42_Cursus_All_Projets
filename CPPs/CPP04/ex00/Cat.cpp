#include "Cat.hpp"

/**
 * RULES FOR DERIVED CLASS CONSTRUCTORS:
 * 1. BASE CLASS FIRST: The base class part of the object is always constructed 
 * before the derived class part.
 * 2. CHAINING: If you don't call a specific base constructor, the compiler 
 * automatically calls the default (parameterless) base constructor.
 * 3. MANDATORY CALLS: If the base class has no default constructor, the 
 * derived class MUST explicitly call one of the available base constructors.
 * 4. INITIALIZATION LIST: Use the list to pass arguments to the base class 
 * and to initialize derived-specific members efficiently.
 */

Cat::Cat(): Animal()
{
	this->type = "Cat";
	std::cout << "[Cat] Default constructor called\n";
}

/**
 * Cat Copy Constructor
 * * We explicitly delegate the copying process to the Base Class (Animal).
 * Since 'other' is a Cat, it is also an Animal (upcasting), so we can 
 * pass it directly to the Animal copy constructor.
 * This ensures all inherited attributes are correctly copied.
 */
Cat::Cat( const Cat& other ) : Animal(other)
{

}

//Explicit Base Call: We MUST use 'Animal::' to avoid inifinite recursion
Cat& Cat::operator=( const Cat& other)
{
	if (this != &other)
		Animal::operator=(other);

	return *this;
}

Cat::~Cat()
{
	std::cout << "[Cat] Destructor called\n";
}

void	Cat::makeSound() const
{
	std::cout << this->type << " Meow! Meow!\n";
}
