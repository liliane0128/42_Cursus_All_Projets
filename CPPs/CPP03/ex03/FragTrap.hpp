#ifndef FRAGTRAP_HPP
# define FRAGTRAP_HPP

#include "ClapTrap.hpp"

/*
 * VIRTUAL INHERITANCE:
 * By using 'virtual public ClapTrap', we ensure that only ONE instance of 
 * the base class (ClapTrap) exists in any future derived classes that use 
 * multiple inheritance (like DiamondTrap). 
 * * Without 'virtual', a DiamondTrap object would contain two separate ClapTraps, 
 * leading to memory waste and compiler ambiguity when accessing shared attributes.
 */
class FragTrap: virtual public ClapTrap
{
public:
	FragTrap( const std::string& name );
	~FragTrap();
	
	void	highFivesGuys( void );
	void	attack( const std::string& target );
};

#endif
