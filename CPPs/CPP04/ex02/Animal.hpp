# ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal
{
protected:
	std::string	type;

public:
	Animal();
	Animal( const Animal& other );
	Animal& operator=( const Animal& other );
	virtual ~Animal();

	std::string getType() const;
	virtual void	makeSound() const = 0;
};
#endif

/*
 * ABSTRACT CLASS SUMMARY:
 * 1. Pure Virtual Function: The '= 0' syntax tells the compiler that 
 * this function has no implementation in the base class.
 *
 * 2. Instantiation: You cannot create an object of an abstract class. 
 * Any attempt to 'new Animal()' will result in a compilation error.
 * 
 * 3. Polymorphism: Abstract classes are designed to be used as 
 * INTERFACES. You use the base class pointer to manage various 
 * derived class objects (Dogs, Cats) uniformly.
 * 
 * 4. Rule of Thumb: If a class has at least one pure virtual 
 * function, it is abstract. If it provides no implementation for 
 * any inherited pure virtual functions, the subclass also becomes abstract.
 */