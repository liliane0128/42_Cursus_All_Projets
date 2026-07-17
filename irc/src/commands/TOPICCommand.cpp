/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPICCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:01:08 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 16:57:06 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/TOPICCommand.hpp"
#include <sstream>

TOPICCommand::TOPICCommand() {}
TOPICCommand::~TOPICCommand() {}

NumericCode TOPICCommand::parse(const std::string& prefix,
									const std::vector<std::string>& params)
{
	// Check for minimum parameters: channel
	if (!hasEnoughParams(params, 1))
		return (ERR_NEEDMOREPARAMS);

	// Set the target channel
	_channel = params[0];

	// Check if a topic is provided (optional for viewing the current topic)
	if (params.size() >= 2)
	{
		// Reconstruct the full topic text (in case it was split by spaces)
		_topic = reconstructTrailingParam(params, 1);
	}
	else
		// No topic provided, this is a request to view the current topic
		_topic = "";

	// [DEBUG] Log topic change attempt
	logCommand(prefix, "Topic change attempt", _channel, _topic);
	
	return (ERR_NONE);
}

void TOPICCommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	Client* client = server.getClient(fd);
	if (!client)
		return;

	// Find the channel
	Channel* channel = server.findChannel(_channel);
	if (!channel)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, _channel);
		return;
	}

	// Check if client is in the channel
	if (!channel->hasClient(*client))
	{
		server.sendNumericMsg(fd, ERR_NOTONCHANNEL, _channel);
		return;
	}

	// If no topic provided, show the current topic
	if (_topic.empty())
	{
		// Check if channel has a topic
		if (channel->getTopic().empty())
			server.sendNumericMsg(fd, RPL_NOTOPIC, _channel);
		else
			server.sendNumericMsg(fd, RPL_TOPIC, _channel, channel->getTopic());
		return;
	}

	// Check if channel is in topic protection mode (+t)
	if (channel->hasMode(MODE_TOPIC) && !channel->isOperator(*client))
	{
		server.sendNumericMsg(fd, ERR_CHANOPRIVSNEEDED, _channel);
		return;
	}

	// Set the new topic
	channel->setTopic(_topic);

	// Build the TOPIC message
	std::string topicMsg = ":" + client->getNickname() + "!"
							+ client->getUsername() + "@"
							+ client->getHostname() + " TOPIC "
							+ _channel + " :" + _topic;

	// Broadcast the topic change to all channel members
	channel->broadcast(topicMsg);
}
