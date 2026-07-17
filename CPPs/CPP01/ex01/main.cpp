/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:58 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:59 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

int main()
{
	int N = 8;

	Zombie* horde = zombieHorde(N, "Demogorgon");

	for(int i = 0; i < N; i++)
	{
		horde[i].announce();
	}

	delete[] horde;

	return 0;
}
