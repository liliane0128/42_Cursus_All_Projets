/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOINCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:04:02 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 13:27:38 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/JOINCommand.hpp"
#include <sstream>

JOINCommand::JOINCommand() {}
JOINCommand::~JOINCommand() {}


NumericCode JOINCommand::parse(const std::string& prefix,
							  const std::vector<std::string>& params)
{
	// Clear previous data to avoid recycling from past calls
	_channels.clear();
	_keys.clear();

	// Check for minimum parameters: at least one channel name
	if (params.empty())
		return (ERR_NEEDMOREPARAMS);

	// Parse channel names
	_channels = splitCommaSeparatedList(params[0]);

	// Validate each channel name
	for (std::vector<std::string>::iterator it =
							_channels.begin(); it != _channels.end(); ++it)
	{
		if (!Channel::isValidChannelName(*it))
			return (ERR_NOSUCHCHANNEL);
	}

	// Parse keys if provided (second parameter)
	if (params.size() >= 2)
		_keys = splitCommaSeparatedList(params[1]);

	// [DEBUG] Log the join attempt
	logCommand(prefix, "Join attempt", reconstructTrailingParam(params, 0));

	return (ERR_NONE);
}

void JOINCommand::execute(int fd,
						const std::vector<std::string>& params, Server& server)
{
	(void)params;
	
	Client* client = server.getClient(fd);
	if (!client) return;

	// Process each channel the client wants to join
	for (size_t i = 0; i < _channels.size(); i++)
	{
		const std::string& channelName = _channels[i];
		Channel* channel = server.findChannel(channelName);

		// Skip if client is already in channel
		if (channel && channel->hasClient(*client))
			continue;

		// Create channel if it doesn't exist
		if (!channel)
		{
			server.createChannel(channelName);
			channel = server.findChannel(channelName);

			// First user becomes operator
			channel->addClient(*client);
			channel->setUserMode(*client, USER_MODE_OPERATOR, true);
		}
		else
		{
			// Check channel restrictions for existing channels
			if (channel->hasMode(MODE_INVITE) && !channel->isInvited(*client))
			{
				server.sendNumericMsg(fd, ERR_INVITEONLYCHAN, channelName);
				continue;
			}

			if (channel->hasMode(MODE_KEY))
			{
				std::string key = (i < _keys.size()) ? _keys[i] : "";
				if (key.empty() || key != channel->getKey())
				{
					server.sendNumericMsg(fd, ERR_BADCHANNELKEY, channelName);
					continue;
				}
			}

			if (channel->hasMode(MODE_LIMIT) &&
				channel->getClients().size() >= channel->getUserLimit())
			{
				server.sendNumericMsg(fd, ERR_CHANNELISFULL, channelName);
				continue;
			}

			// Add client to existing channel
			channel->addClient(*client);
		}

		// Build JOIN message
		std::string joinMsg = ":" + client->getNickname() + "!"
							+ client->getUsername() + "@" 
							+ client->getHostname() + " JOIN :" + channelName;

		// Broadcast JOIN to all channel members
		channel->broadcast(joinMsg);

		// Send channel topic information
		if (!channel->getTopic().empty())
			server.sendNumericMsg(fd, RPL_TOPIC, channelName,
											channel->getTopic());
		else
			server.sendNumericMsg(fd, RPL_NOTOPIC, channelName);

		// Send channel client list
		sendChannelClientList(fd, *channel, server);
	}
}

void JOINCommand::sendChannelClientList(int fd, Channel& channel,
														Server& server)
{
	std::string namesList = "";
	
	// Build names list with prefixes
	for (size_t i = 0; i < channel.getClients().size(); i++)
	{
		Client* c = channel.getClients()[i];
		std::string prefix;

		if (channel.isOperator(*c))
			prefix = "@";
		else
			prefix = "";

		if (!namesList.empty())
			namesList += " ";

		namesList += prefix + c->getNickname();
	}

	// Send channel client name list using sendNumericMsg
	server.sendNumericMsg(fd, RPL_NAMREPLY, "=", channel.getName(), namesList);
	server.sendNumericMsg(fd, RPL_ENDOFNAMES, channel.getName());
}
