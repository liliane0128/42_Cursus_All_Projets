#include "ClapTrap.hpp"

int	main()
{
	ClapTrap clippy("Clippy");
	ClapTrap bandit("Bandit");

    std::cout << "\n--- Battle Starts! ---\n" << std::endl;
	clippy.attack("Bandit");
	bandit.takeDamage(5);
	bandit.beRepaired(3);
	for (int i = 0; i < 10; i++)
		bandit.attack("Clippy");
	bandit.attack("Clippy");

	clippy.takeDamage(20);
	clippy.beRepaired(10);
	clippy.attack("Bandit");
	std::cout << "\n--- The End ---\n" << std::endl;

	return (0);
}
