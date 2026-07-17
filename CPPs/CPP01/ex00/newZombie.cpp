/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newZombie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:39 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:40 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

/**
 * Allocate a new Zombie object on the HEAP using the 'new' keyword.
 * This calls the constructor and reserves memory that persists 
 * beyond the scope of this function.
 */
Zombie* newZombie( std::string name )
{
	Zombie *zombiePtr = new Zombie(name);

	return zombiePtr;
}
