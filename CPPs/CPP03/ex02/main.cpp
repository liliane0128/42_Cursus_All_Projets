#include "FragTrap.hpp"
#include "ScavTrap.hpp"

int main()
{
    FragTrap frag("Fraggy");

    std::cout << "\n--- Battle Starts! ---\n" << std::endl;

    frag.highFivesGuys();    
    frag.attack("someone");
	frag.takeDamage(99);
	frag.beRepaired(1);
    frag.highFivesGuys();
   
   	frag.takeDamage(100);
    frag.highFivesGuys();
    frag.attack("someone");

 	std::cout << "\n--- The End ---\n" << std::endl;

    return 0;
}