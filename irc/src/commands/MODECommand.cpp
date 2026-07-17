/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODECommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 16:01:08 by neandrie          #+#    #+#             */
/*   Updated: 2026/03/27 16:37:37 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/MODECommand.hpp"
#include <sstream>

MODECommand::MODECommand() {}
MODECommand::~MODECommand() {}

NumericCode MODECommand::parse(const std::string& prefix,
									const std::vector<std::string>& params)
{
	// Check for minimum parameters: target
	if (!hasEnoughParams(params, 1))
		return (ERR_NEEDMOREPARAMS);

	// Set the target (channel or nickname)
	_target = params[0];

	// Check if there are mode parameters
	if (params.size() >= 2)
	{
		// The second parameter is the mode string (e.g., "+i", "-k", "+o", "+ikl")
		_modeString = params[1];

		// Check if there are additional params (e.g., keys, nicknames, ...)
		if (params.size() >= 3)
		{
			_modeParams = "";
			for (size_t i = 2; i < params.size(); i++)
			{
				if (!_modeParams.empty())
					_modeParams += " ";
				_modeParams += params[i];
			}
		}
		else
			_modeParams = "";
	}
	else
	{
		// No mode string provided, i.e. request to view the current modes
		_modeString = "";
		_modeParams = "";
	}

	// [DEBUG] Log the mode change attempt
	std::string allInfo = "";
	if (!_modeString.empty())
	{
		allInfo = _modeString;
		if (!_modeParams.empty())
			allInfo += " " + _modeParams;
	}
	logCommand(prefix, "Mode check/change attempt", _target, allInfo);
	return (ERR_NONE);
}

void MODECommand::execute(int fd,
					const std::vector<std::string>&params, Server& server)
{
	(void)params;
	
	Client* client = server.getClient(fd);
	if (!client)
		return;

	// Check if target is a channel (starts with # or &; + and ! not managed)
	bool isChannel = (_target[0] == '#' || _target[0] == '&');

	if (isChannel)
		handleChannelMode(fd, *client, server); // Handle channel modes
	else
		handleUserMode(fd, *client, server); // Handle user modes
}

void MODECommand::handleChannelMode(int fd, Client& client, Server& server)
{
	Channel* channel = server.findChannel(_target);

	// Check if the channel exist
	if (!channel)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHCHANNEL, _target);
		return;
	}

	// Check if the client is in the channel
	if (!channel->hasClient(client))
	{
		server.sendNumericMsg(fd, ERR_NOTONCHANNEL, _target);
		return;
	}

	// If no mode string is provided, return current channel modes
	if (_modeString.empty())
	{
		std::string modeStr = "+";
		bool hasModes = false;

		// Build the mode string
		if (channel->hasMode(MODE_INVITE))
		{
			modeStr += "i";
			hasModes = true;
		}

		if (channel->hasMode(MODE_KEY))
		{
			modeStr += "k";
			hasModes = true;
		}

		if (channel->hasMode(MODE_LIMIT))
		{
			modeStr += "l";
			hasModes = true;
		}

		if (channel->hasMode(MODE_TOPIC))
		{
			modeStr += "t";
			hasModes = true;
		}

		// Build the mode parameters string
		std::string modeParams;
		if (channel->hasKey())
			modeParams = channel->getKey();

		if (channel->hasUserLimit())
		{
			std::ostringstream oss;
			oss << channel->getUserLimit();
			if (!modeParams.empty())
				modeParams += " ";
			modeParams += oss.str();
		}

		// If no modes are set, ensure modeParams is empty
		if (!hasModes || (!channel->hasKey() && !channel->hasUserLimit()))
			modeParams = " ";

		server.sendNumericMsg(fd, RPL_CHANNELMODEIS, _target, modeStr, modeParams);
		return;
	}

	// Check operator privileges for mode changes
	if (!channel->isOperator(client))
	{
		server.sendNumericMsg(fd, ERR_CHANOPRIVSNEEDED, _target);
		return;
	}

	// Check if this is a user mode change (contains 'o' or 'v')
	bool isUserModeChange = false;
	for (size_t i = 0; i < _modeString.size(); i++)
	{
		char c = _modeString[i];
		if (c == '+' || c == '-') continue;
		if (c == 'o')
		{
			isUserModeChange = true;
			break;
		}
	}

	if (isUserModeChange)
	{
		// Check if there's a target nickname
		if (_modeParams.empty())
		{
			server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, _modeString);
			return ;
		}

		// Get the target client
		Client* targetClient = server.findClientByNick(_modeParams);
		if (!targetClient)
		{
			server.sendNumericMsg(fd, ERR_NOSUCHNICK, _modeParams);
			return ;
		}

		// Check if the target client is in the channel
		if (!channel->hasClient(*targetClient))
		{
			server.sendNumericMsg(fd, ERR_USERNOTINCHANNEL, _modeParams, _target);
			return ;
		}
		
		// Parse and apply the user mode changes
		NumericCode result = channel->parseUserModeString(_modeString, *targetClient);
		if (result != ERR_NONE)
		{
			server.sendNumericMsg(fd, result, _modeString);
			return ;
		}

		// Build the MODE message to broadcast
		std::string modeMessage = ":" + client.getNickname() + "!"
									+ client.getUsername() + "@"
									+ client.getHostname() + " MODE "
									+ _target + " " + _modeString
									+ " " + _modeParams;

		// Broadcast the mode change to the channel
		channel->broadcast(modeMessage);
		return ;
	}

	// Handle channel mode changes
	NumericCode result = channel->parseModeString(_modeString, _modeParams);
	if (result != ERR_NONE)
	{
		server.sendNumericMsg(fd, result, _modeString, _target);
		return;
	}

	// Build the MODE message
	std::string modeMessage = ":" + client.getNickname() + "!"
								+ client.getUsername() + "@"
								+ client.getHostname() + " MODE "
								+ _target + " " +
								_modeString;

	if (!_modeParams.empty())
		modeMessage += " " + _modeParams;

	// Broadcast MODE message to channel
	channel->broadcast(modeMessage);
}

void MODECommand::handleUserMode(int fd, Client& client, Server& server)
{
	// Check if target = self
	if (_target != client.getNickname())
	{
		server.sendNumericMsg(fd, ERR_USERSDONTMATCH);
		return;
	}

	// If no mode string is provided, return current modes (+ none or +o operator)
	if (_modeString.empty())
	{
		std::string currentModes = client.getModeString();
		server.sendNumericMsg(fd, RPL_UMODEIS, currentModes);
		return;
	}

	// Apply user mode changes (= none as none are supported). Only OPER enable +o.
	NumericCode result = client.parseModeString(_modeString);
	if (result != ERR_NONE)
	{
		server.sendNumericMsg(fd, result, _modeString);
		return;
	}

	// Never reach as long as we do not managed user mode change
	// (+i is not supported, etc.)
	// Build MODE message
	std::string response = ":" + client.getNickname() + " MODE " + _target
								+ " :" + _modeString;


	server.sendReply(fd, response);
}