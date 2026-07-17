/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICKCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:15:44 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 16:07:40 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/KICKCommand.hpp"

KICKCommand::KICKCommand() {}
KICKCommand::~KICKCommand() {}

NumericCode KICKCommand::parse(const std::string& prefix,
								const std::vector<std::string>& params)
{
	// Check for minimum parameters: channel and nickname
	if (!hasEnoughParams(params, 2))
		return (ERR_NEEDMOREPARAMS);

	// Set the channel
	_channel = params[0];

	// Parse nicknames (can be comma-separated)
	_nicknames = splitCommaSeparatedList(params[1]);

	// Check if there is a comment (optional)
	if (params.size() >= 3)
		// Reconstruct the full comment (in case it was split by spaces)
		_comment = reconstructTrailingParam(params, 2);
	else
		_comment = "";  // No comment provided

	// [DEBUG] Log the kick attempt	
	std::string allInfo =
				_channel + " " + params[1]; // channel + comma-listed nicknames
	if (!_comment.empty())
		allInfo += " : " + _comment;
	logCommand(prefix, "Kick attempt", "", allInfo);

	return (ERR_NONE);
}

void KICKCommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	Client* kicker = server.getClient(fd);
	if (!kicker)
		return;

	// Find the channel
	Channel* channel = server.findChannel(_channel);
	if (!channel)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, _channel);
		return;
	}

	// Check if kicker is in the channel
	if (!channel->hasClient(*kicker))
	{
		server.sendNumericMsg(fd, ERR_NOTONCHANNEL, _channel);
		return;
	}

	// Check if kicker has operator privileges
	if (!channel->isOperator(*kicker))
	{
		server.sendNumericMsg(fd, ERR_CHANOPRIVSNEEDED, _channel);
		return;
	}

	// Process each nickname to kick
	for (size_t i = 0; i < _nicknames.size(); i++)
	{
		const std::string& targetNick = _nicknames[i];
		Client* target = server.findClientByNick(targetNick);

		// Check if target exists
		if (!target)
		{
			server.sendNumericMsg(fd, ERR_NOSUCHNICK, targetNick);
			continue;
		}

		// Check if target is in the channel
		if (!channel->hasClient(*target))
		{
			server.sendNumericMsg(fd, ERR_USERNOTINCHANNEL, targetNick, _channel);
			continue;
		}

		std::string kickMsg = ":" + kicker->getNickname() + "!" + kicker->getUsername() +
							  "@" + kicker->getHostname() + " KICK " + _channel + " " + targetNick;

		// Add comment if provided
		if (!_comment.empty())
			kickMsg += " :" + _comment;

		// Broadcast KICK message to channel
		channel->broadcast(kickMsg);

		// Remove target from channel
		channel->rmClient(*target);
	}
	// If channel is empty after kick, destroy it
	if (channel->getClients().empty())
			server.destroyChannel(_channel);
}
