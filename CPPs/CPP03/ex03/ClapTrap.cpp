#include "ClapTrap.hpp"

ClapTrap::ClapTrap( const std::string& name ): 
name (name), hitPoints(10), energyPoints(10), attackDamage(0)
{
	std::cout << "ClapTrap " << name << " bootstrapped and ready!\n";
}

ClapTrap::~ClapTrap()
{
	std::cout << "ClapTrap " << name << " power source depleted... shutting down.\n";

}

void	ClapTrap::attack( const std::string& target )
{
	if (this->energyPoints <= 0)
	{
		std::cout << "ClapTrap " << this->name << " has no energy to attack\n";
		return ;
	}
	if ( this->hitPoints <= 0)
	{
		std::cout << "ClapTrap " << this->name << " has no hit point to attack\n";
		return ;
	}
	this->energyPoints--;
	std::cout << "ClapTrap " << this->name << " attacks " << target << ", causing "
	<< this->attackDamage << " points of damage!\n";
}

void	ClapTrap::takeDamage( unsigned int amount )
{
	if (amount > static_cast<unsigned int>(this->hitPoints))
		this->hitPoints = 0;
	else
		this->hitPoints -= amount;

	std::cout << "ClapTrap " << this->name << " takes " << amount << " damage!\n";
}


void	ClapTrap::beRepaired( unsigned int amount )
{
	if (this->energyPoints <= 0)
	{
		std::cout << "ClapTrap " << this->name << " has no energy to repaire\n";
		return ;
	}
	if ( this->hitPoints <= 0)
	{
		std::cout << "ClapTrap " << this->name << " has no hit point to repaire\n";
		return ;
	}

	this->energyPoints--;
	this->hitPoints += amount;

	std::cout << "ClapTrap " << this->name << " feels much better now! Restored " 
			  << amount << " hit points!\n";
}
