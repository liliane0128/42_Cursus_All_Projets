/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:42 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:43 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Weapon.hpp"

Weapon::Weapon( std::string type ) : _type(type) {}

Weapon::~Weapon() {}


const std::string& Weapon::getType( void ) const
{
	return _type;
}

void	Weapon::setType(const std::string& newType)
{
	_type = newType;
}
