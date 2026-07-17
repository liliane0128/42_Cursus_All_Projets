#include "DiamondTrap.hpp"

/*
 * DIAMONDTRAP CONSTRUCTOR:
 * 1. Explicitly initialize ClapTrap because it is a virtual base class.
 * The DiamondTrap is solely responsible for this initialization.
 * 2. Initialize direct parents (FragTrap, ScavTrap).
*/
DiamondTrap::DiamondTrap( const std::string& name ):
ClapTrap(name + "_clap_name"),
FragTrap(name),
ScavTrap(name)
{
	this->name = name;
	this->hitPoints = FragTrap::hitPoints; //100
	this->energyPoints = ScavTrap::energyPoints; //50
	this->attackDamage = FragTrap::attackDamage; //30

	std::cout << "DiamondTrap " << this->name << " - The ultimate fusion is born!!\n";
}

DiamondTrap::~DiamondTrap()
{
	std::cout << "DiamondTrap " << this->name << " - Even perfection has an end!\n";
}

void	DiamondTrap::whoAmI()
{
	std::cout << "My name is " << this->name << ", and my ClapTrap name is "
	<< ClapTrap::name << std::endl;
}

//Explicitly call ScavTrap::attack() as requested by the subject
void	DiamondTrap::attack( const std::string& target )
{
	ScavTrap::attack(target);
}