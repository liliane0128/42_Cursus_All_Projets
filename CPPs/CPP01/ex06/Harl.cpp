/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:36 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:37 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Harl.hpp"

void	Harl::debug( void )
{
	std::cout << "[ DEBUG ]\n" << DEBUG_MSG << std::endl;
}

void	Harl::info( void )
{
	std::cout << "[ INFO ]\n" << INFO_MSG << std::endl;
}

void	Harl::warning( void )
{
	std::cout << "[ WARNING ]\n" << WARN_MSG << std::endl;
}
void	Harl::error( void )
{
	std::cout << "[ ERROR ]\n" << ERROR_MSG << std::endl;
}

// fall-through intentional
void	Harl::complain( std::string level )
{
	std::string levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

	int	i = 0;

	while (i < 4 && levels[i] != level)
		i++;
	switch (i)
	{
		case 0:
			this->debug();
		case 1:
			this->info();
		case 2:
			this->warning();
		case 3:
			this->error();
			break;
		default:
			std::cout << "[ Probably complaining about insignificiant problems ]" 
					  << std::endl;
	}
}
