/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:01 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:02 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

void Zombie::setName (const std::string& name )
{
	_name = name;
}

Zombie::~Zombie ()
{
	std::cout << _name << " died" << std::endl;
}

void	Zombie::announce( void )
{
	std::cout << _name << ": "
			  << "BraiiiiiiinnnzzzZ..." << std::endl;
}
