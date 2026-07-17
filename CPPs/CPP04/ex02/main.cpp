#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int main()
{
	// These will cause compiler error cause we can't instantiate an Animal
	//Animal test;
	//Animal* ptr = new Animal();

	{
		std::cout << "=== 1. Polymorphic Array Test (Abstract Class) ===\n\n";
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

	{
		std::cout << "\n=== 2. Deep Copy Verification ===\n\n";
		Dog basic;
		basic.getBrain()->setIdeas(0, "I want to play fetch");
		
		{
			Dog tmp = basic;
			std::cout << std::endl;

			std::cout << "Tmp idea: " << tmp.getBrain()->getIdeas(0) << std::endl;
			
			tmp.getBrain()->setIdeas(0, "I want to sleep");                                 
			std::cout << "Tmp modified idea: " << tmp.getBrain()->getIdeas(0) << std::endl;
		} 
		std::cout << std::endl;

		std::cout << "Basic original idea: " << basic.getBrain()->getIdeas(0) << std::endl;
	}

    {
		std::cout << "\n=== 3. Assignment Operator Test ===\n\n";
		Cat cat1;
		cat1.getBrain()->setIdeas(0, "I'm hungry");
		Cat cat2;
		cat2 = cat1;
		
		std::cout << std::endl;

		std::cout << "Cat 2 idea: " << cat2.getBrain()->getIdeas(0) << std::endl;
		cat1.getBrain()->setIdeas(0, "I'm not hungry anymore!");
		std::cout << "Now: Cat 1 idea: " << cat1.getBrain()->getIdeas(0) << std::endl;
		std::cout << "Cat 2 still has: " << cat2.getBrain()->getIdeas(0) << std::endl;

		std::cout << std::endl;

	}

	return 0;
}

