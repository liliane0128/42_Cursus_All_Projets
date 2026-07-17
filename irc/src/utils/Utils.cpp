/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 14:04:55 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 14:37:48 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/Utils.hpp"

namespace Utils
{
	std::vector<std::string> splitCommaSeparatedList(const std::string& list)
	{
		std::vector<std::string> result;
		if (list.empty())
			return (result);

		size_t start = 0;
		size_t commaPos = list.find(',');

		while (commaPos != std::string::npos)
		{
			// Extract substring between commas
			std::string token = list.substr(start, commaPos - start);
			if (!token.empty())
				result.push_back(token);
			start = commaPos + 1;
			commaPos = list.find(',', start);
		}
		// Add the last element after the final comma (or the single element)
		if (start < list.length())
		{
			std::string token = list.substr(start);
			if (!token.empty())
				result.push_back(token);
		}
		return (result);
	}

		std::vector<std::string> splitSpaceSeparatedList(const std::string& list)
	{
		std::vector<std::string> result;
		if (list.empty())
			return (result);

		size_t start = 0;
		size_t spacePos = list.find(' ');

		while (spacePos != std::string::npos)
		{
			// Extract substring between space
			std::string token = list.substr(start, spacePos - start);
			if (!token.empty())
				result.push_back(token);
			start = spacePos + 1;
			spacePos = list.find(',', start);
		}
		// Add the last element after the final space (or the single element)
		if (start < list.length())
		{
			std::string token = list.substr(start);
			if (!token.empty())
				result.push_back(token);
		}
		return (result);
	}

	void validatePort(const std::string &portStr, int &port)
	{
		for (size_t i = 0; i < portStr.size(); i++)
		{
			if (!std::isdigit(portStr[i]))
				throw std::invalid_argument(
					"Port must be a numeric value (e.g. 6667).");
		}

		errno = 0;
		long parsed = std::strtol(portStr.c_str(), NULL, 10);

		if (errno == ERANGE || parsed < 1 || parsed > 65535)
			throw std::out_of_range(
				"Port must be in range 1-65535 (TCP valid range).");

		// Ports 0-1023 are privileged on POSIX systems
		// IRC conventionally runs on 6667 (plaintext) per RFC 7194
		if (parsed < 1024)
			throw std::out_of_range(
				"Port " + portStr + " is in the privileged range (0-1023). "
				"Privileged ports require root on POSIX systems. ");

		port = static_cast<int>(parsed);
	}

	void validatePassword(const std::string &password)
	{
		if (password.empty())
			throw std::invalid_argument("Password must not be empty.");

		// RFC 1459 §2.3 — password must not contain NUL, CR, LF, or spaces
		for (size_t i = 0; i < password.size(); i++)
		{
			char c = password[i];
			if (c == '\0' || c == '\r' || c == '\n' || c == ' ')
				throw std::invalid_argument(
					"Password must not contain NUL, CR, LF, or space characters.");
		}
	}
}