#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type("General WrongAnimal")
{
	std::cout << "[WrongAnimal] Default constructor called\n";
}

WrongAnimal::~WrongAnimal()
{
	std::cout << "[WrongAnimal] Destructor called\n";
}

WrongAnimal::WrongAnimal( const WrongAnimal& other ) : type(other.type)
{

}

WrongAnimal& WrongAnimal::operator=( const WrongAnimal& other )
{
	if (this != &other)
		this->type = other.getType();

	return *this;
}

void	WrongAnimal::makeSound() const
{
	std::cout << "The WrongAnimal makes a noise\n";
}

std::string WrongAnimal::getType() const
{
	return (this->type);
}
