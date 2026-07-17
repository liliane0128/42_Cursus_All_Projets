/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:07:04 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/09 15:02:07 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include "Contact.hpp"

class Phonebook
{
private:
	Contact _contacts[8];
	int		_count;
	std::string format_string(std::string str);

public:
	//Constructor
	Phonebook() : _count(0) {}

	void	add_contact();
	void	search_contact();
};

#endif
