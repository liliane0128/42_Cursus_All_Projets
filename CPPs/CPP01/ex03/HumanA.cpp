/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:20 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:21 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanA.hpp"

HumanA::HumanA(std::string name, Weapon& weapon): 
	_name(name),
	_weapon(weapon) {}

HumanA::~HumanA() {}


void	HumanA::attack( void ) const
{
	std::cout << _name << " attacks with their "
			  << _weapon.getType() << std::endl;
}
