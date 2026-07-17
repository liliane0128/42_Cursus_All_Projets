/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 10:35:00 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 19:22:04 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/Colors.hpp"
#include "commands/CommandParser.hpp"
#include "commands/DCCCommand.hpp"
#include "commands/INVITECommand.hpp"
#include "commands/JOINCommand.hpp"
#include "commands/KICKCommand.hpp"
#include "commands/LISTCommand.hpp"
#include "commands/MODECommand.hpp"
#include "commands/NICKCommand.hpp"
#include "commands/OPERCommand.hpp"
#include "commands/PARTCommand.hpp"
#include "commands/PASSCommand.hpp"
#include "commands/PINGCommand.hpp"
#include "commands/PRIVMSGCommand.hpp"
#include "commands/QUITCommand.hpp"
#include "commands/TOPICCommand.hpp"
#include "commands/USERCommand.hpp"
#include "commands/WHOISCommand.hpp"

/**
 * Static Command Instances
 * ------------------------
 * We declare these command handlers as 'static' at the file scope to ensure:
 * 1. Lifetime: They are initialized once when the program starts and live 
 * until it terminates. No manual new/delete is required.
 * 2. Internal Linkage: These instances are only visible within this translation 
 * unit (CommandParser.cpp), preventing naming conflicts.
 * 3. Memory Safety: Since the CommandParser only needs the logic (not unique
 * state per command), sharing a single global instance is memory-efficient and 
 * avoids complex pointer management in the Parser's destructor.
 */
static DCCCommand dccCommand;
static INVITECommand inviteCommand;
static JOINCommand joinCommand;
static KICKCommand kickCommand;
static LISTCommand listCommand;
static MODECommand modeCommand;
static NICKCommand nickCommand;
static OPERCommand operCommand;
static PARTCommand partCommand;
static PASSCommand passCommand;
static PINGCommand pingCommand;
static PRIVMSGCommand privmsgCommand;
static QUITCommand quitCommand;
static TOPICCommand topicCommand;
static USERCommand userCommand;
static WHOISCommand whoisCommand;

CommandParser::CommandParser()
{
	registerHandler("DCC", &dccCommand);
	registerHandler("INVITE", &inviteCommand);
	registerHandler("JOIN", &joinCommand);
	registerHandler("KICK", &kickCommand);
	registerHandler("LIST", &listCommand);
	registerHandler("MODE", &modeCommand);
	registerHandler("NICK", &nickCommand);
	registerHandler("OPER", &operCommand);
	registerHandler("PART", &partCommand);
	registerHandler("PASS", &passCommand);
	registerHandler("PING", &pingCommand);
	registerHandler("PRIVMSG", &privmsgCommand);
	registerHandler("QUIT", &quitCommand);
	registerHandler("TOPIC", &topicCommand);
	registerHandler("USER", &userCommand);
	registerHandler("WHOIS", &whoisCommand);
}

CommandParser::~CommandParser() {}

void CommandParser::dispatchCommand(int fd, const std::string& rawInput,
									Server& server)
{
	// Log the comment received
	std::cout << BLUE <<  "[Received] " << rawInput << RESET << std::endl;
	ParsedMessage msg = parseParams(rawInput);
	if (msg.command.empty())
		return ;

	// Normalize command to uppercase
	std::string normalizedCommand = msg.command;
	for (size_t i = 0; i < normalizedCommand.size(); ++i)
		normalizedCommand[i] = toupper(normalizedCommand[i]);

	// Ignore CAP (capability negotiation), not required for a basic IRC
	if (normalizedCommand == "CAP")
		return ;
		
	// Look up the normalized command
	std::map<std::string, CommandHandler*>::iterator it =
										handlers.find(normalizedCommand);
	if (it != handlers.end())
	{	// Delegate parsing and execution to the handler
		it->second->processCommand(msg.prefix, fd, msg.params, server);
		return ;
	}
	server.sendNumericMsg(fd, ERR_UNKNOWNCOMMAND, normalizedCommand);
	
	return ;
}

void CommandParser::registerHandler(const std::string& name,
								CommandHandler* handler)
{
	handlers[name] = handler;
}

ParsedMessage CommandParser::parseParams(const std::string& rawInput)
{
	ParsedMessage msg;
	size_t i = 0;

	// Check for prefix (leading ':')
	if (!rawInput.empty() && rawInput[0] == ':')
	{
		size_t spacePos = rawInput.find(' ', 1);
		if (spacePos == std::string::npos)
			return msg; // Invalid message: only prefix, no command
		msg.prefix = rawInput.substr(1, spacePos - 1);
		i = spacePos + 1; // Skip prefix and space
	}

	// Parse command
	size_t nextSpace = rawInput.find(' ', i);
	if (nextSpace == std::string::npos)
		nextSpace = rawInput.size();
	msg.command = rawInput.substr(i, nextSpace - i);
	i = nextSpace + 1; // Skip command and space

	// Parse parameters
	std::string current;
	bool inTrailingParam = false;
	for (; i < rawInput.size(); ++i)
	{
		if (rawInput[i] == ' ')
		{
			if (!current.empty())
			{
				msg.params.push_back(current);
				current.clear();
			}
		}
		else if (rawInput[i] == ':' && !inTrailingParam)
		{
			inTrailingParam = true;
			current += rawInput.substr(i + 1);
			break;
		}
		else
			current += rawInput[i];
	}
	if (!current.empty())
		msg.params.push_back(current);

	// Enforce 15-parameter limit (excluding prefix and command)
	if (msg.params.size() > 15)
	{
		// Truncate to 15 parameters
		msg.params.resize(15);
	}

	return (msg);
}
