/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:06:51 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/07 16:06:52 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"
#include "Phonebook.hpp"

int main()
{
	Phonebook book;
	std::string cmd;

	while (true)
	{
		std::cout << "Enter command (ADD, SEARCH, EXIT): ";
		if (!std::getline(std::cin, cmd) || cmd == "EXIT")
			break ;
		if (cmd.empty())
			continue ;
		else if (cmd == "ADD")
			book.add_contact();
		else if (cmd == "SEARCH")
			book.search_contact();
		else
			std::cout << "Invalid command!" << std::endl;
	}
	return 0;
}
