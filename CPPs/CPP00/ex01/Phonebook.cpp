/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:06:58 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/07 16:06:59 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Phonebook.hpp"

/**
 * Formats a string to fit the 10-character column requirement:
 * If the string is longer than 10 chars, it truncates to 9 chars 
 * and appends a dot '.'.
 */
std::string Phonebook::format_string(std::string str)
{
	if (str.length() > 10)
		return str.substr(0,9) + '.';
	return str;
}

void	Phonebook::add_contact()
{
	size_t	i = _count % 8;
	std::cout << "Adding contact to index " << (i + 1) << std::endl;
	_contacts[i].init_contact();
	_count++;
}

/**
 * std::setw(10) - "Set Width" is a Stream Manipulator:
 * Ensures the NEXT output field occupies at least 10 characters.
 * If the content is shorter than 10, the remaining space is filled 
 * with padding (default is spaces). This is essential for creating
 * Right-alignment. 
 * 
 * * std::atoi requires a C-style string (const char*).
 * Since 'input' is a std::string object, we use .c_str() to get
 * a compatible pointer for the conversion to an integer.
 */
void	Phonebook::search_contact()
{
	int limit = (_count > 8) ? 8 : _count;
	if (limit == 0)
	{
		std::cout << "The Phonebook is empty" << std::endl;
		return ;
	}
	std::cout << "|" << std::setw(10) << "Index" << "|" << std::setw(10) << "First Name" 
		<< "|" << std::setw(10)	<< "Last Name" << "|" << std::setw(10) << "Nickname"
		<< "|" << std::endl;
	for (int i = 0; i < limit; i++)
	{
		std::cout << "|" << std::setw(10) << i + 1;
		std::cout << "|" << std::setw(10) << format_string(_contacts[i].get_first_name());
		std::cout << "|" << std::setw(10) << format_string(_contacts[i].get_last_name());
		std::cout << "|" << std::setw(10) << format_string(_contacts[i].get_nickname());
		std::cout << "|" << std::endl;
	}
	std::cout << "Please enter the index of the contact to display all the information" << std::endl;
	std::string input = get_input("Enter Index: ");
	if (is_numeric(input))
	{
		int index = std::atoi(input.c_str());
		if (index >= 1 && index <= limit)
			_contacts[index - 1].display_all();
		else
			std::cout << "Index out of range" << std::endl;
	}
	else
		std::cout << "Invalid index: must be a number." << std::endl;
}
