/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:28 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:29 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanB.hpp"

HumanB::HumanB( std::string name ) :
	_name(name),
	_weapon(NULL) {}

HumanB::~HumanB() {}

void	HumanB::setWeapon(Weapon& weapon)
{
	_weapon = &weapon;
}

void	HumanB::attack( void ) const
{
	if (_weapon == NULL)
		std::cout << _name << " has no weapon to attack with!" << std::endl;
	else
		std::cout << _name << " attacks with their "
			  	  << _weapon->getType() << std::endl;
}
