/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITECommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:10:16 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/09 15:07:38 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/INVITECommand.hpp"

INVITECommand::INVITECommand() {}
INVITECommand::~INVITECommand() {}

NumericCode INVITECommand::parse(const std::string& prefix,
								const std::vector<std::string>& params)
{
	// Check for minimum parameters: nickname and channel
	if (!hasEnoughParams(params, 2))
		return (ERR_NEEDMOREPARAMS);

	// Set the nickname and channel
	_nickname = params[0];
	_channel = params[1];

	// [DEBUG] Log the invite attempt
	logCommand(prefix, "Invite attempt", _channel, _nickname);

	return (ERR_NONE);
}

void INVITECommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	
	Client* inviter = server.getClient(fd);
	if (!inviter)
		return ;

	// Find the target client to invite
	Client* target = server.findClientByNick(_nickname);
	if (!target)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHNICK, _nickname);
		return ;
	}

	// Find the channel
	Channel* channel = server.findChannel(_channel);
	if (!channel)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, _channel);
		return ;
	}

	// Check if inviter is in the channel
	if (!channel->hasClient(*inviter))
	{
		server.sendNumericMsg(fd, ERR_NOTONCHANNEL, _channel);
		return ;
	}

	// Check if inviter has operator privileges
	if (!channel->isOperator(*inviter))
	{
		server.sendNumericMsg(fd, ERR_CHANOPRIVSNEEDED, _channel);
		return ;
	}

	// Check if target is already in the channel
	if (channel->hasClient(*target))
	{
		server.sendNumericMsg(fd, ERR_USERONCHANNEL, _nickname, _channel);
		return ;
	}

	// Add the target to the invite list
	channel->inviteClient(*target);

	// Send RPL_INVITING to the inviter
	server.sendNumericMsg(fd, RPL_INVITING, _channel, _nickname);

	// Build INVITE message
	std::string inviteMsg = ":" + inviter->getNickname() + "!"
							+ inviter->getUsername() +
							"@" + inviter->getHostname() + " INVITE "
							+ _nickname + " :" + _channel;

	// Notify the target about the invitation
	server.sendReply(target->getFd(), inviteMsg);
}

