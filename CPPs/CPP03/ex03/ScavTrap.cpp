#include "ScavTrap.hpp"

//Explicitly call parent constrcutor to pass parameters
ScavTrap::ScavTrap( const std::string& name ): ClapTrap(name)
{
	//Parent parts are initialized first, then we customize the values here
	this->hitPoints = 100;
	this->energyPoints = 50;
	this->attackDamage = 20;
	std::cout << "ScavTrap " << name << " has entered Sentry Mode!\n";
}

ScavTrap::~ScavTrap()
{
	std::cout << "ScavTrap " << this->name << " abandoning post... guard duty over.\n";
	//Note: the destructor of parent is automaticcaly called by the compiler after
	// this block. In C++, Base class destructor are always called implicitly in reverse order
}

void	ScavTrap::guardGate()
{
	if (this->hitPoints <= 0)
	{
		std::cout << "ScavTrap " << this->name << " cannot guard gate: it's already scrap metal\n";
		return ;
	}
	if (this->energyPoints <= 0)
	{
		std::cout << "ScavTrap " << this->name << " is out of energy and can't stay awake to guard!\n";
		return ;
	}
	
	std::cout << "ScavTrap " << this->name << " is now in Gate kepper mode\n";
}

void	ScavTrap::attack( const std::string& target )
{
	if (this->energyPoints <= 0)
	{
		std::cout << "ScavTrap " << this->name << " has no energy to attack\n";
		return ;
	}
	if ( this->hitPoints <= 0)
	{
		std::cout << "ScavTrap " << this->name << " has no hit point to attack\n";
		return ;
	}
	this->energyPoints--;
	std::cout << "ScavTrap " << this->name << " attacks " << target << ", causing "
	<< this->attackDamage << " points of damage!\n";
}
