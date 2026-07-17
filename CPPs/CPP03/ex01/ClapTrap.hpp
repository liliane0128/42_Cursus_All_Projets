#ifndef CLAPTRAP_HPP
# define CLAPTRAP_HPP

#include <iostream>
#include <string>

class ClapTrap
{
/* 'protected' access specifier:
 * Members are private to the outside world but accessible to derived
 * classes (like ScavTrap), This allows child classes to directly 
 * modify these attributes without needing public setter functions.
*/	
protected:
	std::string name;
	int			hitPoints;
	int			energyPoints;
	int			attackDamage;

public:
	ClapTrap( const std::string& name );
	
	/*
     * 'virtual' destructor:
     * Essential for inheritance. It ensures that when a derived object 
     * is deleted through a pointer to the base class, the derived class 
     * destructor is called first, preventing memory leaks.
     */
	virtual ~ClapTrap();
	
	/*
     * 'virtual' member function:
     * Enables "Dynamic Binding" (Polymorphism). It tells the compiler 
     * to look for the most specific version of this function at runtime. 
     * If ScavTrap overrides this, the ScavTrap version will be called 
     * even when using a ClapTrap pointer.
     */
	virtual void	attack( const std::string& target );
	void	takeDamage( unsigned int amount );
	void	beRepaired( unsigned int amount );
};

#endif