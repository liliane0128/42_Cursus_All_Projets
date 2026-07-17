/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 15:21:00 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 18:19:36 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include "commands/CommandHandler.hpp"
#include "utils/IRCNumericCode.hpp"
#include <map>

struct ParsedMessage
{
	std::string prefix;				// Empty if no prefix
	std::string command;			// Always present if message is valid
	std::vector<std::string> params;// Parameters (excluding prefix and command)
};

class CommandParser
{
	public:
		CommandParser();
		~CommandParser();

		// Dispatch command for next step, i.e. parameters parsing and execution
		void dispatchCommand(int fd,
							const std::string& rawInput, Server& server);


	private:
		std::map<std::string, CommandHandler*> handlers;

		// Register a command handler
		void registerHandler(const std::string& name, CommandHandler* handler);

		// Helper to parse raw input into command + params
		ParsedMessage parseParams(const std::string& rawInput);

};

#endif