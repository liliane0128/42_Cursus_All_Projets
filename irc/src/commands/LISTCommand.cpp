/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LISTCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:57:47 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 16:53:53 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/LISTCommand.hpp"
#include <sstream>

LISTCommand::LISTCommand() {}
LISTCommand::~LISTCommand() {}

NumericCode LISTCommand::parse(const std::string& prefix,
								const std::vector<std::string>& params)
{
	(void)params;

	// [DEBUG] Log the LIST attempt
	logCommand(prefix, "LIST attempt", "all channels", "Requesting channel list");

	return ERR_NONE;
}

void LISTCommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	Client* client = server.getClient(fd);
	if (!client) return;

	std::string clientNick = client->getNickname();

	// Get all channels from the server
	const std::map<std::string, Channel>& channels = server.getChannels();

	// Iterate through all channels
	for (std::map<std::string, Channel>::const_iterator it = channels.begin();
		 it != channels.end(); ++it)
	{
		const Channel& channel = it->second;

		// Get channel information
		std::string channelName = channel.getName();
		std::string topic = channel.getTopic();
		size_t userCount = channel.getClients().size();

		// Convert userCount to string
		std::ostringstream oss;
		oss << userCount;
		std::string userCountStr = oss.str();

		// Send channel information
		if (!topic.empty())
		{
			server.sendNumericMsg(fd, RPL_LIST,
				channelName,
				userCountStr,
				topic);
		}
		else
		{
			server.sendNumericMsg(fd, RPL_LIST,
				channelName,
				userCountStr,
				"topic is yet to be set");
		}
	}

	// Send LIST end
	server.sendNumericMsg(fd, RPL_LISTEND, clientNick);
}
