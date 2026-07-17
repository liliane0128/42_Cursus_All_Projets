# ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP

#include <iostream>
#include <string>

//Without 'virtual' keyword, we can't have the POLYMORPHISM
class WrongAnimal
{
protected:
	std::string	type;

public:
	WrongAnimal();
	WrongAnimal( const WrongAnimal& other );
	WrongAnimal& operator=( const WrongAnimal& other );
	~WrongAnimal();

	std::string getType() const;
	void	makeSound() const;
};
#endif

/**
 * THE CRITICAL ROLE OF 'VIRTUAL':
 * 1. DYNAMIC VS STATIC BINDING: Without 'virtual', the compiler uses Static Binding. 
 * It looks at the type of the POINTER (e.g., Animal*), not the actual OBJECT 
 * it points to (e.g., Cat). Thus, it would call Animal::makeSound() every time.
 * 
 * * 2. V-TABLE MECHANISM: Adding 'virtual' creates a "Virtual Table" (vtable). 
 * At runtime, the program checks this table to find the specific implementation 
 * of the derived class (Dynamic Dispatch), enabling true Polymorphism.
 * 
 * * 3. WRONGANIMAL CONTRAST: This is why WrongAnimal fails to output "Meow"; 
 * it lacks the 'virtual' keyword, so the compiler is "blind" to the Cat's 
 * overridden method when accessed via a WrongAnimal* pointer.
 * 
 * * 4. MEMORY SAFETY: Without a virtual destructor, 'delete animalPtr' would 
 * only call the Animal destructor, leading to memory leaks if the Cat 
 * part of the object had allocated memory (like a Brain).
 */