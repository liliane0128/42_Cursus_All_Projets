/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:15 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:16 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

/**
 * * @brief Reference: An alias for the string.
 * - Acts as a permanent 'bound' label to the original variable.
 *
 * * Unlike a pointer, a reference:
 * - cannot be reassigned to refer to another object.
 * - is accessed directly without dereferencing (*).
 */

int main()
{
	std::string str = "HI THIS IS BRAIN";
	std::string* stringPTR = &str;
	std::string& stringREF = str;

	std::cout << "Momory address of the string variable: " << &str << std::endl;
	std::cout << "Momory address held by stringPTR:      " << stringPTR << std::endl;
	std::cout << "Momory address held by stringREF:      " << &stringREF << std::endl;

	std::cout << std::endl;

	std::cout << "Value of the string variable:  " << str << std::endl;
	std::cout << "Value pointed to by stringPTR: " << *stringPTR << std::endl;
	std::cout << "Value pointed to by stringREF: " << stringREF << std::endl;

	return 0;
}

