#include "ScavTrap.hpp"

/*
 * POLYMORPHISM:
 * Even though 'ref' is a reference to a ClapTrap, it points to a ScavTrap object.
 * Because 'attack' is virtual, the program checks the actual object type at 
 * runtime and calls ScavTrap::attack(). The "Bandit" identity is preserved.
 */
int main()
{
	ScavTrap bandit("Bandit");
	ClapTrap& ref = bandit;

    std::cout << "\n--- Battle Starts! ---\n" << std::endl;

	bandit.attack("Clippy");
	ref.attack("Clippy");
	bandit.guardGate();

    std::cout << std::endl;

	ClapTrap* army[2];
	army[0] = new ClapTrap("BasicBot");
	army[1] = new ScavTrap("EliteGuard");

	for (int i = 0; i < 2; i++)
		army[i]->attack("Target Dummy");
	
	for (int i = 0; i < 2; i++)
		delete army[i];

	std::cout << "\n--- The End ---\n" << std::endl;

	return 0;
}