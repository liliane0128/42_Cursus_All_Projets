#include "DiamondTrap.hpp"

int main()
{
    DiamondTrap dede("Dede");

    std::cout << "\n--- Game Starts! ---\n" << std::endl;

    dede.whoAmI();
    dede.attack("enemy");
    dede.guardGate();
    dede.highFivesGuys();
	dede.takeDamage(30);
	dede.beRepaired(10);

	//after death
    std::cout<< std::endl;

	dede.takeDamage(100);
	dede.beRepaired(10);
    dede.attack("enemy");
	dede.guardGate();
    dede.highFivesGuys();


    std::cout << "\n--- Game Over :( ---\n" << std::endl;

    return 0;
}