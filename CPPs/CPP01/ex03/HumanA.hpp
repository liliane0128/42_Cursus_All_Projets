/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:24 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:25 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANA_CPP
#define HUMANA_CPP

#include "Weapon.hpp"

/**
 * HumanA uses a reference to Weapon,
 * so it must be initialized in the constructor and can never be null,
 * which guarantees that HumanA is always armed.
 */

class HumanA
{
private:
	std::string	_name;
	Weapon& _weapon;

public:

	HumanA( std::string name, Weapon& weapon );
	~HumanA();

	void	attack( void ) const;
};
#endif
