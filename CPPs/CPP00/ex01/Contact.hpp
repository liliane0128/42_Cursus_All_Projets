/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:06:45 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/09 15:03:09 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_H
# define CONTACT_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include "Utils.hpp"

/**
 * CLASS DEFINITION AND ACCESS CONTROL
 * * A Class serves as a blueprint or template that defines the strcture and 
 * the behavior of an Object. It groups data (attributes) and functions (methods)
 * together into a single logical unit.
 * 
 * * - PRIVATE (Encapsulation)
 * Only accessible from within the class, protecting the object's internal state
 * from accidental modification and ensuring data integrity.
 * 
 * * - PUBLIC (Interface)
 * Accessible from outside the class.
 */

class Contact
{
private:
	std::string first_name;
	std::string last_name;
	std::string nickname;
	std::string phone_number;
	std::string darkest_secret;

public:
	void		init_contact();
	std::string get_first_name() const;
	std::string	get_last_name() const;
	std::string	get_nickname() const;
	void		display_all() const;
};

#endif