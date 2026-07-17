#include "Cat.hpp"

Cat::Cat(): Animal()
{
	this->brain = new Brain();
	this->type = "Cat";
	std::cout << "[Cat] Default constructor called\n";
}

/**
 * DEEP COPY instead of a Shallow Copy:
 * Why use *other.brain?
 * 1. DEREFERENCING: 'other.brain' is a pointer (an address). By using the '*' 
 * operator, we access the actual Brain OBJECT stored at that address.
 * 2. INDEPENDENT ALLOCATION: 'new Brain(*other.brain)' allocates a fresh 
 * memory block on the heap and calls the Brain copy constructor to 
 * replicate the content.
 * 3. AVOIDING SHARED STATE: If we didn't use '*', both Cat objects would 
 * point to the same Brain. When one Cat is deleted, the other Cat's brain 
 * would become a "Dangling Pointer," leading to a crash.
 */
Cat::Cat( const Cat& other ) : Animal(other)
{
	if (other.brain)
		this->brain = new Brain(*other.brain);
	else
		this->brain = NULL;
}

/**
 * THE COPY-AND-SWAP IDIOM:
 * 1. EXCEPTION SAFETY: By allocating the new 'Brain' BEFORE deleting the 
 * old one, we ensure that if 'new' fails, the original object remains 
 * in a valid, unchanged state.
 * 2. NO DANGLING POINTERS: This sequence ensures 'this->brain' 
 * always points to a valid Brain object, even if an error occurs.
 */
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
