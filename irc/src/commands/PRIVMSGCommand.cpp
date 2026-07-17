/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSGCommand.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 16:08:46 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 16:56:40 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/PRIVMSGCommand.hpp"
#include <sstream>

PRIVMSGCommand::PRIVMSGCommand() {}
PRIVMSGCommand::~PRIVMSGCommand() {}

NumericCode PRIVMSGCommand::parse(const std::string& prefix,
									const std::vector<std::string>& params)
{
	// Check for minimum parameters: target and text
	if (params.empty())
		return (ERR_NORECIPIENT); // No target provided

	_target = params[0];

	// Check if text is provided (params[1] may start with : for trailing text)
	if (params.size() < 2)
		return (ERR_NOTEXTTOSEND);
	
	// Reconstruct the full text (in case it was split by spaces)
	_text = reconstructTrailingParam(params, 1);

	// [DEBUG] Log the message attempt
	logCommand(prefix, "Message attempt", _target, _text);

	return (ERR_NONE);
}

void PRIVMSGCommand::execute(int fd,
								const std::vector<std::string>& params,
								Server& server)
{
	(void)params;

	Client* sender = server.getClient(fd);
	if (!sender)
		return ;

	if (_target.empty())
		return ;

	bool isChannel = (_target[0] == '#' || _target[0] == '&');

	// CHANNEL MESSAGE
	if (isChannel)
	{
		// Check if target channel exists
		Channel* channel = server.findChannel(_target);
		if (!channel)
		{
			server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, _target);
			return ;
		}

		// Check if sender can send to channel
		if (!channel->hasClient(*sender))
		{
			server.sendNumericMsg(fd, ERR_CANNOTSENDTOCHAN, _target);
			return ;
		}

		// Build the PRIVMSG message
		std::string privMsg = ":" + sender->getNickname() + "!" +
							  sender->getUsername() + "@" +
							  sender->getHostname() + " PRIVMSG " +
							  _target + " :" + _text;

		// Broadcast PRIVMSG message to channel
		channel->broadcast(privMsg, sender);
		return ;
	}

	// USER MESSAGE
	Client* targetClient = server.findClientByNick(_target);
	if (!targetClient)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHNICK, _target);
		return ;
	}
	
	// Build the PRIVMSG message
	std::string message = ":" + sender->getNickname() + "!" +
						  sender->getUsername() + "@" +
						  sender->getHostname() + " PRIVMSG " +
						  _target + " :" + _text;

	// Send PRIVMSG message to target
	server.sendReply(targetClient->getFd(), message);
	return ;
}
