/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomChump.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:43 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:44 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

//Stack Allocation
void randomChump( std::string name )
{
	//Allocating the object directly on the stack.
	Zombie z(name);

	z.announce();

	// No need to manuaaly delete.
	// The destructor is called automatically when 'z' goes out of scope.
}
