/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:06:39 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/09 15:35:12 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

/**
 * * Scope Resolution Operator (Contact ::) (Opérateur de Résolution de Portée)
 * Used to define a member function outside its class declaration.
 * It tells the compiler that this function belongs to the Contact class.
 * 
 * * 'this' is a pointer to the current object instance.
 * Using 'this->first_name' explicitly refers to the member variable
 * of the class, distinguishing it from local variables or parameters.
 * Note: if member variables are predixed with '_', 'this->' can be omitted.
 */
void	Contact::init_contact()
{
	this->first_name = get_input("First Name: ");
	if (std::cin.eof())	return;
	this->last_name = get_input("Last Name: ");
	if (std::cin.eof())	return;
	this->nickname = get_input("Nickname: ");
	if (std::cin.eof())	return;

	while (true)
	{
		this->phone_number = get_input("Phone Number: ");
		if (std::cin.eof())	return;

		if (is_numeric(this->phone_number))
			break ;
		std::cout << "Phone Number must be numbers" << std::endl;
	}

	this->darkest_secret = get_input("Darkest Secret: ");
	if (std::cin.eof())	return;
}


std::string Contact::get_first_name() const
{
	return this->first_name;
}

std::string	Contact::get_last_name() const
{
	return this->last_name;
}

std::string	Contact::get_nickname() const
{
	return this->nickname;
}

void	Contact::display_all() const
{
	std::cout << "First name: " << this->first_name << std::endl;
	std::cout << "Last name: " << this->last_name << std::endl;
	std::cout << "Nickname: " << this->nickname << std::endl;
	std::cout << "Phone Number: " << this->phone_number << std::endl;
	std::cout << "Darkest Secret: " << this->darkest_secret << std::endl;
}