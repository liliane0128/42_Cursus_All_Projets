/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:24 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:25 by lilwang          ###   ########.fr       */
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

/*
 * * Harl::complain
 * ----------------
 * This function takes a string 'level' and calls the corresponding member function
 * (debug, info, warning, or error) of the Harl object.
 *
 * Explanation of member function pointer:
 * - 'void (Harl::*methods[])(void)' declares an array of pointers to member functions
 *   of the Harl class that take no arguments and return void.
 * - Each element of 'methods' stores the address of one of Harl's member functions,
 *   using the syntax '&Harl::functionName'.
 * - To call a member function pointer, we use the syntax '(this->*methods[i])();',
 *   which invokes the function on the current object (*this).
 * - This approach allows selecting and calling a member function dynamically at runtime
 *   based on the string 'level', avoiding multiple if-else or switch statements.
 */

void	Harl::complain( std::string level )
{
	std::string levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};

	void (Harl::*methods[])(void) =
	{
		&Harl::debug, //the adress of the func debug()
		&Harl::info,
		&Harl::warning,
		&Harl::error
	};

	for (int i = 0; i < 4; i++)
	{
		if (levels[i] == level)
		{
			(this->*methods[i])();
			return ;
		}
	}
	std::cout << "[ Probably complaining about insignificiant problems ]" 
			  << std::endl;
}
