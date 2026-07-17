#include "FragTrap.hpp"

FragTrap::FragTrap( const std::string& name ): ClapTrap( name )
{
	this->hitPoints = 100;
	this->energyPoints = 100;
	this->attackDamage = 30;

	std::cout << "FragTrap " << name << " enters the battlefield!\n";
}

FragTrap::~FragTrap()
{
	std::cout << "FragTrap " << name << " falls apart!\n";
}

void	FragTrap::attack( const std::string& target )
{
	if (this->energyPoints <= 0)
	{
		std::cout << "FragTrap " << this->name << " has no energy to attack\n";
		return ;
	}
	if ( this->hitPoints <= 0)
	{
		std::cout << "FragTrap " << this->name << " has no hit point to attack\n";
		return ;
	}
	this->energyPoints--;
	std::cout << "FragTrap " << this->name << " attacks " << target << ", causing "
	<< this->attackDamage << " points of damage!\n";
}

void	FragTrap::highFivesGuys( void )
{
	if (this->hitPoints <= 0)
	{
		std::cout << "FragTrap " << this->name << " cannot high five because it's dead!\n";
		return ;
	}
	if (this->energyPoints <= 0)
	{
		std::cout << "FragTrap " << this->name << " is too tired for high fives...\n";
		return ;
	}

	std::cout << "FragTrap " << this->name << " says: High five, everyone!\n";

}
