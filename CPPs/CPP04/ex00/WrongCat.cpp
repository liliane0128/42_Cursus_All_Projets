#include "WrongCat.hpp"

WrongCat::WrongCat(): WrongAnimal()
{
	this->type = "WrongCat";
	std::cout << "[WrongCat] Default constructor called\n";
}

WrongCat::WrongCat( const WrongCat& other ) : WrongAnimal(other)
{

}

WrongCat& WrongCat::operator=( const WrongCat& other )
{
	if (this != &other)
		WrongAnimal::operator=(other);
	return *this;
}

WrongCat::~WrongCat()
{
	std::cout << "[WrongCat] Destructor called\n";
}

void	WrongCat::makeSound() const
{
	std::cout << this->type << " Meooooooooooow!\n";
}