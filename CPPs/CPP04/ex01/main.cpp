#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Brain.hpp"

int main()
{
	{
		std::cout << "=== 1. Polymorphic Array & Memory Leak Test ===\n\n";
		const int count = 4;
		Animal* animals[count];

		for (int i = 0; i < count; i++)
		{
			if (i < count / 2)
				animals[i] = new Dog();
			else
				animals[i] = new Cat();
		}

		std::cout << "\n-----------------\n" << std::endl;

		for (int i = 0; i < count; i++)
			animals[i]->makeSound();

		std::cout << "\n-----------------\n" << std::endl;

		for (int i = 0; i < count; i++)
			delete animals[i];
	}



	std::cout << "\n=== 2. DEEP COPY TEST ===\n\n";
	{
		Dog basic;
		Dog doggy;
		basic.getBrain()->setIdeas(0, "I want a bone");
		{
			Dog tmp = basic;
			doggy = basic;
			std::cout << std::endl;
			std::cout << "Tmp idea: " << tmp.getBrain()->getIdeas(0) << std::endl;
			std::cout << "Doggy idea: " << doggy.getBrain()->getIdeas(0) << std::endl;


			tmp.getBrain()->setIdeas(0, "I want to sleep");
			std::cout << "Tmp idea changed to " << tmp.getBrain()->getIdeas(0) << std::endl;
			std::cout << std::endl;
		}
		std::cout << "Basic original idea: " << basic.getBrain()->getIdeas(0) << std::endl;
	}



	std::cout << "\n=== 3. SCOPE & DESTRUCTION TEST ===\n\n";
	{
		std::cout << "Creating a temporary Cat in a sub-scope..." << std::endl;
		{
			Cat tempCat;
			tempCat.makeSound();
		}
		std::cout << "Temporary Cat should be destroyed by now." << std::endl;
	}

	return 0;
}
