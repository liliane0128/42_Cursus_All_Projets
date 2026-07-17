/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:35 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:36 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

int main()
{
	randomChump("Stack_Zombie");

	Zombie* heapZ = newZombie("Heap_Zombie");
	heapZ->announce();

	delete heapZ;

	return 0;
}
