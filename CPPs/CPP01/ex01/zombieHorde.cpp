/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombieHorde.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:09 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:10 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

Zombie* zombieHorde( int N, std::string name )
{
	Zombie* horde = new Zombie[N]; //array of objects

	for (int i = 0; i < N; i++)
		horde[i].setName(name);
		//horde[i] is an object, so we use '.' as the operator.
	
	return horde;
}
