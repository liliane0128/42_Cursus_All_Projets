/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:47 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:48 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

//Using Member Initializer List for better performance
Zombie::Zombie( const std::string& name ) :_name(name)
{
	// & (By Reference):
	// The & ensures we use the original string's address instead of creating a new copy
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
