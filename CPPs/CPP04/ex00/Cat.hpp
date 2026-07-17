#ifndef CAT_HPP
# define CAT_HPP

#include "Animal.hpp"

class Cat: public Animal
{
public:
	Cat();
	Cat( const Cat& other );
	Cat& operator=( const Cat& other );
	virtual ~Cat();

	void	makeSound() const;
};

#endif

/**
 * RULE FOR DERIVED CLASSES (Copy Constructor & Assignment):
 * * 1. WHEN TO USE COMPILER DEFAULT:
 * - If the derived class only adds simple data types (int, float, bool) 
 * or classes that handle their own memory (std::string, std::vector).
 * - The compiler will automatically call the base class copy logic.
 * 
 * * 2. WHEN TO RE-IMPLEMENT MANUALLY:
 * - If the derived class adds "Raw Pointers" (e.g., Brain* brain = new Brain()).
 * - If you need custom logic during copying.
 * 
 * * 3. CRITICAL REQUIREMENT IF RE-IMPLEMENTING:
 * - You MUST explicitly call the Base Class versions:
 * - Constructor: Cat(const Cat& other) : Animal(other) { ... }
 * - Assignment: Animal::operator=(other);
 * 
 * - Failure to do so will result in the Base Class part being default-initialized 
 * instead of copied.
 */