/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 16:07:07 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/09 15:18:34 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

bool	is_only_whitespace(std::string str)
{
	if (str.empty())
		return true;
	for (int i = 0; str[i]; i++)
	{
		if (!std::isspace(static_cast<unsigned char>(str[i])))
			return false;
	}
	return true;
}

/**
 * * std::getline reads the entire line including spaces,
 * whereas it would stop at the first whitespace and leave the rest in the buffer.
 * It also allow us to detect EOF(Ctrl+D) through the return value.
 */
std::string get_input(std::string input)
{
	std::string res;
	while (true)
	{
		std::cout << input;
		if (!std::getline(std::cin, res))
			return "";
		if (!res.empty() && !is_only_whitespace(res))
			break ;
		std::cout << input << " can't be empty" << std::endl;
	}
	return res;
}

bool	is_numeric(std::string str)
{
	if (str.empty())
		return (false);
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}
