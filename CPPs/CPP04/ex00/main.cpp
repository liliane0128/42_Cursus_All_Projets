#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongCat.hpp"

int main()
{
	{
		std::cout << "=== 1. Basic Polymorphism Test ===\n\n";
		const Animal* meta = new Animal();
		const Animal* j = new Dog();
		const Animal* i = new Cat();

		std::cout << "\n-----------------\n" << std::endl;

		std::cout << j->getType() << " " << std::endl;
		std::cout << i->getType() << " " << std::endl;
		i->makeSound();
		j->makeSound();
		meta->makeSound();
		
		std::cout << "\n-----------------\n" << std::endl;
		delete meta;
		delete i;
		delete j;
	}


	{
		std::cout << "\n=== 2. Copy and Assignment Test ===\n\n";
		Cat catty;
		Cat copyCat(catty);
		std::cout << "Original Type: " << catty.getType() << std::endl;
		std::cout << "Copy Type: " << copyCat.getType() << std::endl;

		std::cout << std::endl;

		Dog dog1;
		Dog dog2;
		dog2 = dog1;
		std::cout << "Dog2 Type: " << dog2.getType() << std::endl;
	}
	

	{
		std::cout << "\n=== 3. Scope and Destruction Test ===\n" << std::endl;
		Dog stackDog;
		stackDog.makeSound();
	}

	{
    	std::cout << "\n=== 4. WrongAnimal Test ===\n" << std::endl;
		const WrongAnimal* wrong = new WrongCat();
		wrong->makeSound();
		delete wrong;
	}

	return 0;
}

