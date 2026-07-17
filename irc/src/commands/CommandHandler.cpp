/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:04:57 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/12 14:20:36 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include <iostream>

CommandHandler::~CommandHandler() {}

// Default implementation of processCommand that calls parse and then execute
void CommandHandler::processCommand(const std::string& prefix, int fd,
										const std::vector<std::string>& params,
										Server& server)
{
	// First parse the command
	NumericCode parseResult = parse(prefix, params);
	if (parseResult != ERR_NONE)
	{
		server.sendNumericMsg(fd, parseResult);
		return ;
	}

	// If parsing succeeded, execute the command
	execute(fd, params, server);
}

// Reconstructs a parameter string from the given start index,
// handle trailing parameters (prefixed with ':')
std::string CommandHandler::reconstructTrailingParam(
								const std::vector<std::string>& params,
								size_t startIndex) const
{
	if (params.size() <= startIndex)
		return ("");
		
	// Reconstruct the full text (in case it was split by spaces)
	std::string result = params[startIndex];
	if (!result.empty() && result[0] == ':')
	{
		// If parameter starts with ':', concatenate all remaining parameters
		for (size_t i = startIndex + 1; i < params.size(); ++i)
			result += " " + params[i];
		// Remove the leading ':' if present (RFC 2812 §2.3.1)
		if (!result.empty() && result[0] == ':')
			result = result.substr(1);
	}
	return (result); // params[startIndex]+ silently discarded
}


bool CommandHandler::hasEnoughParams(const std::vector<std::string>& params,
										size_t minParams) const
{
	return params.size() >= minParams;
}

// Splits a comma-separated string into a vector of strings
std::vector<std::string> CommandHandler::splitCommaSeparatedList(
											const std::string& list) const
{
	std::vector<std::string> result;
	if (list.empty())
		return (result);
	size_t start = 0;
	size_t commaPos = list.find(',');
	while (commaPos != std::string::npos)
	{
		// Extract substring between commas
		result.push_back(list.substr(start, commaPos - start));
		start = commaPos + 1;
		commaPos = list.find(',', start);
	}
	// Add the last element after the final comma
	if (start < list.length())
		result.push_back(list.substr(start));
	return (result);
}

// Provides visibility of the command processing flow
void CommandHandler::logCommand(const std::string& prefix,
									const std::string& commandName,
									const std::string& target,
									const std::string& additionalInfo) const
{
	if (!prefix.empty())
	{
		if (!additionalInfo.empty())
			std::cout << commandName << " from " << prefix
				<< " for " << target << ": " << additionalInfo << std::endl;
		else
			std::cout << commandName << " from " << prefix
				<< " for " << target << std::endl;
	}
	else
	{
		if (!additionalInfo.empty())
			std::cout << commandName
				<< " for " << target << ": " << additionalInfo << std::endl;
		else
			std::cout << commandName
				<< " for " << target << std::endl;
	}
}