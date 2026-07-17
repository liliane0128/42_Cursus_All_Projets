#ifndef DIAMONDRAP_HPP
# define DIAMONDRAP_HPP

#include "FragTrap.hpp"
#include "ScavTrap.hpp"

/*
 * AMBIGUITY RESOLUTION:
 * In multiple inheritance, if two parent classes share a function with the 
 * same signature, the compiler cannot decide which one to call.
 *
 * By writing 'using ScavTrap::attack;', we explicitly bring ScavTrap's 
 * implementation into the DiamondTrap scope. This resolves the conflict 
 * without needing to override the function manually.
 */
class DiamondTrap: public FragTrap, public ScavTrap
{
private:
	std::string name;

public:
	DiamondTrap( const std::string& name );
	virtual ~DiamondTrap();

	void	attack( const std::string& target );
	void	whoAmI();

};

#endif