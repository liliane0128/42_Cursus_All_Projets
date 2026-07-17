/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:33 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:34 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANB_CPP
#define HUMANB_CPP

#include "Weapon.hpp"

/**
 * HumanB uses a pointer to Weapon, which can be null,
 * so it may not always have a weapon and can receive it later via a setter.
 */

class HumanB
{
private:
	std::string	_name;
	Weapon* _weapon;

public:
	HumanB( std::string name );
	~HumanB();

	void	setWeapon(Weapon& weapon);
	void	attack( void ) const;

};
#endif
