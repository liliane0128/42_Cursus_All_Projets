/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PARTCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:20:40 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 13:28:10 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "commands/PARTCommand.hpp"

PARTCommand::PARTCommand() {}
PARTCommand::~PARTCommand() {}

NumericCode PARTCommand::parse(const std::string& prefix,
								const std::vector<std::string>& params)
{
	// Clear previous data to avoid recycling from past calls
	_channels.clear();
	
	// Check for minimum parameters: at least one channel name
	if (!hasEnoughParams(params, 1))
		return (ERR_NEEDMOREPARAMS);

	// Parse channel names (can be comma-separated)
	_channels = splitCommaSeparatedList(params[0]);

	// Check if there is a reason (optional)
	if (params.size() >= 2)
		// Reconstruct the full reason (in case it was split by spaces)
		_reason = reconstructTrailingParam(params, 1);
	else
		_reason = "";  // No reason provided

	// [DEBUG] Log the part attempt
	std::string allInfo = params[0]; // comma-listed channels
	if (!_reason.empty())
		allInfo += " : " + _reason;
	logCommand(prefix, "Part attempt", "", allInfo);

	return (ERR_NONE);
}

void PARTCommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	
	Client* client = server.getClient(fd);
	if (!client)
		return;

	std::string clientNick = client->getNickname();

	// Process each channel the client wants to part from
	for (size_t i = 0; i < _channels.size(); i++)
	{
		const std::string& channelName = _channels[i];
		Channel* channel = server.findChannel(channelName);

		// Check if channel exists
		if (!channel)
		{
			server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, channelName);
			continue;
		}

		// Check if client is in the channel
		if (!channel->hasClient(*client))
		{
			server.sendNumericMsg(fd, ERR_NOTONCHANNEL, channelName);
			continue;
		}

		// Build PART message
		std::string partMsg = ":" + clientNick + "!" + client->getUsername() +
							 "@" + client->getHostname() + " PART " + channelName;

		// Add reason if provided
		if (!_reason.empty())
			partMsg += " :" + _reason;

		// Broadcast PART message to channel
		channel->broadcast(partMsg);

		// Remove client from channel
		channel->rmClient(*client);

		// If channel is empty after client leaves, destroy it
		if (channel->getClients().empty())
			server.destroyChannel(channelName);
	}
}