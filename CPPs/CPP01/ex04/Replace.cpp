/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replace.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:12 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:13 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Replace.hpp"

//Replaces all occurrences of the substring s1 in 'content' with the string s2.
std::string replaceAll(std::string content, const std::string& s1, const std::string& s2)
{
	if (s1.empty())
		return content;
	
	std::string res;
	size_t pos = 0;
	size_t found;

	while ((found = content.find(s1, pos)) != std::string::npos)
	{
		// Append the part of the string BEFORE the match (from current 'pos' to 'found')
		res.append(content, pos, found - pos);
		res.append(s2);
		pos = found + s1.length();
	}
	res.append(content, pos, std::string::npos);
	return res;
}
