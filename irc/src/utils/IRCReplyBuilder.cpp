/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplyBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 16:10:24 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 09:44:41 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/IRCReplyBuilder.hpp"
#include "server/Server.hpp"

#include <iostream>
#include <sstream>
// #include <sys/socket.h>

IRCReplyBuilder::IRCReplyBuilder() {
	initNumericCodeTemplates();
}

IRCReplyBuilder::~IRCReplyBuilder()
{}

void IRCReplyBuilder::initNumericCodeTemplates()
{
	// Replies (RPL)
	NumericCodeTemplates[RPL_WELCOME] =           // 001
		":Welcome to the Internet Relay Network {0}!{1}@{2}";
		// <nick> <user> <host>

	NumericCodeTemplates[RPL_YOURHOST] =          // 002
		":Your host is {0}, running version {1}"; 
		// <servername> <version>

	NumericCodeTemplates[RPL_CREATED] =           // 003
		"This server was created {0}";
		// <date>

	NumericCodeTemplates[RPL_MYINFO] =            // 004
		"{0} {1} {2} {3}";
		// <servername> <version> <user modes> <channel modes>

	NumericCodeTemplates[RPL_UMODEIS] =           // 221
		"{0}";
		// <user mode string>

	NumericCodeTemplates[RPL_WHOISUSER] =         // 311
		"{0} {1} {2} * :{3}";
		// <nick> <user> <host> * :<real name>

	NumericCodeTemplates[RPL_WHOISSERVER] =       // 312
		"{0} {1} :{2}";
		// <nick> <server> :<server info>

	NumericCodeTemplates[RPL_ENDOFWHOIS] =        // 318
		"{0} :End of WHOIS list";
		// <nick> :End of WHOIS list

	NumericCodeTemplates[RPL_WHOISCHANNEL] =      // 319
		"{0} :{1}";
		// <nick> :<channel list>

	NumericCodeTemplates[RPL_LIST] =			  // 322
		"{0} {1} :{2}";
		// // <channel> <# users> :<topic>

	NumericCodeTemplates[RPL_LISTEND] =			  // 323
		":End of LIST";
		// :End of LIST
		
	NumericCodeTemplates[RPL_CHANNELMODEIS] =     // 324
		"{0} {1} {2}";
		// <channel> <mode> <mode params>

	NumericCodeTemplates[RPL_NOTOPIC] =           // 331
		"{0} :No topic is set";
		// <channel> :No topic is set

	NumericCodeTemplates[RPL_TOPIC] =             // 332
		"{0} :{1}";
		// <channel> :<topic>

	NumericCodeTemplates[RPL_INVITING] =          // 341
		"{0} {1}";
		// <channel> <nick>

	NumericCodeTemplates[RPL_WHOREPLY] =          // 352
		"{0} {1} {2} {3} {4} {5} :{6} {7}";
		// <channel> <user> <host> <server> <nick> <flags> 
		// :<hopcount> <real name>

	NumericCodeTemplates[RPL_NAMREPLY] =          // 353
		"{0} {1} :{2}";
		// <symbol> <channel> :<user_list>

	NumericCodeTemplates[RPL_ENDOFNAMES] =        // 366
		"{0} :End of NAMES list";
		// <channel> :End of NAMES list

	NumericCodeTemplates[RPL_YOUREOPER] =         // 381
		"{0} :You are now an IRC operator";
		// <nick> :You are now an IRC operator

	// Errors (ERR)
	NumericCodeTemplates[ERR_NOSUCHNICK] =        // 401
		"{0} :No such nick/channel";
		// <nick/channel> :No such nick/channel

	NumericCodeTemplates[ERR_NOSUCHCHANNEL] =     // 403
		"{0} :No such channel";
		// <channel> :No such channel

	NumericCodeTemplates[ERR_CANNOTSENDTOCHAN] =  // 404
		"{0} :Cannot send to channel";
		// <channel> :Cannot send to channel

	NumericCodeTemplates[ERR_NORECIPIENT] =       // 411
		":No recipient given ({0})";
		// <command> :No recipient given

	NumericCodeTemplates[ERR_NOTEXTTOSEND] =      // 412
		":No text to send";
		// :No text to send

	NumericCodeTemplates[ERR_UNKNOWNCOMMAND] =    // 421
		"{0} :Unknown command";
		// <command> :Unknown command

	NumericCodeTemplates[ERR_NONICKNAMEGIVEN] =   // 431
		":No nickname given";
		// :No nickname given

	NumericCodeTemplates[ERR_ERRONEUSNICKNAME] =  // 432
		"{0} :Erroneous nickname";
		// <nick> :Erroneous nickname

	NumericCodeTemplates[ERR_NICKNAMEINUSE] =     // 433
		"{0} :Nickname is already in use";
		// <nick> :Nickname is already in use

	NumericCodeTemplates[ERR_NICKCOLLISION] =     // 436
		"{0} :Nickname collision KILL from {1}@{2}";
		// <nick> :Nickname collision KILL from <user>@<host>

	NumericCodeTemplates[ERR_USERNOTINCHANNEL] =  // 441
		"{0} {1} :They aren't on that channel";
		// <nick> <channel> :They aren't on that channel

	NumericCodeTemplates[ERR_NOTONCHANNEL] =      // 442
		"{0} :You're not on that channel";
		// <channel> :You're not on that channel

	NumericCodeTemplates[ERR_USERONCHANNEL] =     // 443
		"{0} {1} :is already on channel";
		// <user> <channel> :is already on channel

	NumericCodeTemplates[ERR_NEEDMOREPARAMS] =     // 461
		"{0} :Not enough parameters";
		// <command> :Not enough parameters

	NumericCodeTemplates[ERR_ALREADYREGISTRED] =  // 462
		":Unauthorized command (already registered)";
		// :Unauthorized command (already registered)

	NumericCodeTemplates[ERR_PASSWDMISMATCH] =    // 464
		":Password incorrect";
		// :Password incorrect

	NumericCodeTemplates[ERR_CHANNELISFULL] =    // 471
		"{0} :Cannot join channel (+l)";
		// <channel> :Cannot join channel (+l)

	NumericCodeTemplates[ERR_UNKNOWNMODE] =       // 472
		"{0} :is unknown mode char to me for {1}";
		// <char> :is unknown mode char to me for <channel>

	NumericCodeTemplates[ERR_INVITEONLYCHAN] =    // 473
		"{0} :Cannot join channel (+i)";
		// <channel> :Cannot join channel (+i)

	NumericCodeTemplates[ERR_BANNEDFROMCHAN] =    // 474
		"{0} :Cannot join channel (+b)";
		// <channel> :Cannot join channel (+b)

	NumericCodeTemplates[ERR_BADCHANNELKEY] =     // 475
		"{0} :Cannot join channel (+k)";
		// <channel> :Cannot join channel (+k)

	NumericCodeTemplates[ERR_NOCHANMODES] =       // 477
		"{0} :Channel doesn't support modes";
		// <channel> :Channel doesn't support modes

	NumericCodeTemplates[ERR_NOPRIVILEGES] =      // 481
		":Permission Denied- You're not an IRC operator";
		// :Permission Denied- You're not an IRC operator

	NumericCodeTemplates[ERR_CHANOPRIVSNEEDED] =  // 482
		"{0} :You're not channel operator";
		// <channel> :You're not channel operator

	NumericCodeTemplates[ERR_CANTKILLSERVER] =    // 483
		":You can't kill a server!";
		// :You can't kill a server!

	NumericCodeTemplates[ERR_RESTRICTED] =        // 484
		":Your connection is restricted!";
		// :Your connection is restricted!

	NumericCodeTemplates[ERR_UMODEUNKNOWNFLAG] =  // 501
		":Unknown MODE flag";
		// :Unknown MODE flag

	NumericCodeTemplates[ERR_USERSDONTMATCH] =    // 502
		":Cannot change mode for other users";
		// :Cannot change mode for other users

	NumericCodeTemplates[ERR_NICKPOLICYVIOLATION] =    // 901
		"{0} :Nickname policy violation (underscore suffix not allowed by server design)";
		// <nick> :Nickname policy violation (underscore suffix not allowed by server design)
}

std::string IRCReplyBuilder::applyTemplate(const std::string& tmpl,
								const std::vector<std::string>& args) const
{
	std::string result = tmpl;
	for (size_t i = 0; i < args.size(); i++)
	{
		std::ostringstream placeholder;
		placeholder << "{" << i << "}";
		std::string token = placeholder.str();
		size_t pos = 0;
		while ((pos = result.find(token, pos)) != std::string::npos)
		{
			result.replace(pos, token.size(), args[i]);
			pos += args[i].size();
		}
	}
	return result;
}

std::string IRCReplyBuilder::formatNumericReply(int code,
						const std::string& target,
						const std::string& message) const
{
	std::ostringstream oss;
	oss << ":" << "ircserver" << " " << code << " "
		<< target << " " << message << "\r\n";
	return oss.str();
}

//to be review cause it crashed 
void IRCReplyBuilder::sendNumericReply(int clientSocket,
									   int code,
									   const std::string& target,
									   const std::vector<std::string>& args,
									   const Server& server) const
{
	NumericCodeStringMap::const_iterator it = NumericCodeTemplates.find(code);
	if (it == NumericCodeTemplates.end())
	{ // no template defined
		std::cerr << "Error: no template for numeric code "
			<< code << " — reply not sent to " << target << std::endl;
		return ;
	}
	
	if (!server.hasClient(clientSocket))
	{
		std::cerr << "Warning: client FD " << clientSocket << " not found\n";
		return ;
	}
	
	std::string message = applyTemplate(it->second, args);
	std::string reply = formatNumericReply(code, target, message);
 
	if (send(clientSocket, reply.c_str(), reply.length(), 0) == -1)
		std::cerr << "Error: Failed to send message to FD: "
									<< clientSocket << std::endl;
	
	// Print log message without \r\n
	std::string logMsg = reply;
	while (logMsg.size() >= 2 && logMsg.substr(logMsg.size() - 2) == "\r\n")
		logMsg.erase(logMsg.size() - 2);
	std::cout << "[Send numeric reply] " << logMsg << std::endl; 
}