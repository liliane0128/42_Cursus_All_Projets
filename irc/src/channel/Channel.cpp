/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:39:10 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 17:02:45 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "channel/Channel.hpp"
#include "utils/Utils.hpp"
#include <algorithm>  // For std::find
#include <sys/socket.h> // For send()
#include <iostream>
#include <climits> // For LONG_MAX

Channel::Channel(const std::string& name)
	: _name(name), _topic(""), _modes(MODE_NONE), _key(""), _userLimit(0)
{}

// Channel mode management
void Channel::setMode(ChannelMode mode, bool enable)
{
	if (enable)
		_modes |= mode; // Set the mode bit
	else
		_modes &= ~mode; // Clear the mode bit
}

bool Channel::hasMode(ChannelMode mode) const
{
	return (_modes & mode) != 0;
}

NumericCode Channel::parseModeString(const std::string& modeString,
											const std::string& modeParams)
{
	bool adding = true; // Adding mode by default if no +/-
	size_t modeParamIdx = 0;

	std::vector<std::string> params
							= Utils::splitSpaceSeparatedList(modeParams);

	for (size_t i = 0; i < modeString.size(); i++)
	{
		char c = modeString[i];
		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else
		{
			// Map the character to a ChannelMode enum
			ChannelMode mode = MODE_NONE;
			switch (c)
			{
				case 'i': mode = MODE_INVITE;	break;
				case 't': mode = MODE_TOPIC;	break;
				case 'k': mode = MODE_KEY;		break;
				case 'l': mode = MODE_LIMIT;	break;
				default:
					return (ERR_UNKNOWNMODE);
			}
			
			// Handle mode parameters (key for +k)
			if (mode == MODE_KEY && adding)
			{
				if (modeParamIdx >= params.size()
											|| params[modeParamIdx].empty())
					return (ERR_NEEDMOREPARAMS);

				_key = params[modeParamIdx];
				modeParamIdx++;
			}
			else if (mode == MODE_KEY && !adding)
			{
				// Remove the key when -k is used
				_key = "";
			}
			
			// Handle mode parameters (limit for +l)
			else if (mode == MODE_LIMIT && adding)
			{
				if (modeParamIdx >= params.size()
											|| params[modeParamIdx].empty())
					return (ERR_NEEDMOREPARAMS);

				// Check if the parameter is a valid positive number
				char* endptr;
				long limit = strtol(params[modeParamIdx].c_str(), &endptr, 10);

				if (*endptr != '\0' || endptr == params[modeParamIdx].c_str())
					return (ERR_UNKNOWNMODE);

				if (limit <= 0 || limit > INT_MAX)
					return ERR_UNKNOWNMODE;

				_userLimit = static_cast<size_t>(limit);
				modeParamIdx++;
			}
			else if (mode == MODE_LIMIT && !adding)
			{
				// Remove the user limit when -l is used
				_userLimit = 0;
			}

			// Set the specific mode
			setMode(mode, adding);
		}
	}
	return (ERR_NONE);
}

// User mode management
void Channel::setUserMode(Client& client, UserMode mode, bool enable)
{
	int& clientModes = _userModes[&client];
	if (enable)
		clientModes |= mode;  // Set the mode bit
	else
		clientModes &= ~mode; // Clear the mode bit
}

bool Channel::hasUserMode(const Client& client, UserMode mode) const
{
	std::map<Client*, int>::const_iterator it
			= _userModes.find(const_cast<Client*>(&client));
	if (it == _userModes.end())
		return (false);
	return ((it->second & mode) != 0);
}

bool Channel::isOperator(const Client& client) const
{
	return (hasUserMode(client, USER_MODE_OPERATOR));
}

NumericCode		Channel::parseUserModeString(const std::string& userModeString,
									Client& client)
{
	bool adding = true; // Adding mode by default if no +/-

	for (size_t i = 0; i < userModeString.size(); i++)
	{
		char c = userModeString[i];
		if (c == '+')
			adding = true;
		else if (c == '-')
			adding = false;
		else
		{
			// Map the character to a UserMode enum
			UserMode mode = USER_MODE_NONE;
			switch (c)
			{
				case 'o': mode = USER_MODE_OPERATOR; break;
				default:
					return ERR_UNKNOWNMODE;
			}

			// Set the specific user mode
			setUserMode(client, mode, adding);
		}
	}
	return ERR_NONE;
}
										

// Topic management
void Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

const std::string& Channel::getTopic() const
{
	return (_topic);
}

// Name management
void Channel::setName(const std::string& name)
{
	_name = name;
}

const std::string& Channel::getName() const
{
	return (_name);
}

// Client management
void Channel::addClient(Client& client)
{
	if (!hasClient(client))
	{
		_clients.push_back(&client);
		client.joinChannel(*this);
	}
}

void Channel::rmClient(Client& client)
{
	std::vector<Client*>::iterator it
			= std::find(_clients.begin(), _clients.end(), &client);
	if (it != _clients.end())
	{
		_clients.erase(it);
		client.leaveChannel(*this);
	}
	// Remove user modes for this client
	_userModes.erase(&client);
}

bool Channel::hasClient(const Client& client) const
{
	std::vector<Client*>::const_iterator it
		= std::find(_clients.begin(), _clients.end(), &client);
	return (it != _clients.end());
}

const std::vector<Client*>& Channel::getClients() const
{
	return (_clients);
}

// Channel key (password) management
void Channel::setKey(const std::string& key)
{
	_key = key;
}

bool Channel::hasKey() const
{
	return (!_key.empty());
}

const std::string& Channel::getKey() const
{
	return (_key);
}

// Invitation management
void Channel::inviteClient(const Client& client)
{
	_invitedNicks.insert(client.getNickname());
}

void Channel::revokeInvitation(const Client& client)
{
	_invitedNicks.erase(client.getNickname());
}

bool Channel::isInvited(const Client& client) const
{
	return _invitedNicks.find(client.getNickname()) != _invitedNicks.end();
}

bool Channel::hasInvitation(const std::string& nick) const
{
	return _invitedNicks.find(nick) != _invitedNicks.end();
}

// User limit management
void Channel::setUserLimit(size_t limit)
{
	_userLimit = limit;
}

size_t Channel::getUserLimit() const
{
	return (_userLimit);
}

bool Channel::hasUserLimit() const
{
	return (_userLimit > 0);
}

void	Channel::broadcast(const std::string& message)
{
	std::string msg = message;
	std::string logMsg = message;
	if (msg.size() < 2 || msg.substr(msg.size() - 2) != "\r\n")
		msg += "\r\n";

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		Client* client = _clients[i];
		if (!client)
			continue;
		if (send(client->getFd(), msg.c_str(), msg.length(), 0) == -1)
			std::cerr << "Error: Failed to send message to FD: "
					  << client->getFd() << std::endl;

		// Print log message without \r\n
		std::cout << "[Send broadcast] " << logMsg << std::endl; 
	}
}

void	Channel::broadcast(const std::string& message, const Client* sender)
{
	std::string msg = message;
	std::string logMsg = message;
	if (msg.size() < 2 || msg.substr(msg.size() - 2) != "\r\n")
		msg += "\r\n";

	for (size_t i = 0; i < _clients.size(); ++i)
	{
		Client* client = _clients[i];
		if (!client || client == sender)
			continue;
		if (send(client->getFd(), msg.c_str(), msg.length(), 0) == -1)
			std::cerr << "Error: Failed to send message to FD: "
					  << client->getFd() << std::endl;

		// Print log message without \r\n
		std::cout << "[Send broadcast] " << logMsg << std::endl; 
	}
}

bool Channel::isValidChannelName(const std::string& name)
{
	// Check minimum length and starting character
	if (name.empty() || name.size() < 2 || (name[0] != '#' && name[0] != '&'))
		// "!" and "+" channelid are not supported by this ft_irc server
		return (false);

	for (size_t i = 1; i < name.size(); ++i)
	{
		char c = name[i];
		// As per RFC 2812, §2.3.1
		// Forbidden: NUL, BELL, CR, LF, space, comma, colon
		if (c == '\0' ||  c == '\a' || c == '\r' || c == '\n'
				|| c == ' ' || c == ',' || c == ':')
			return (false);
	}

	// As per RFC 2812, §1.3
	if (name.size() > 50)
		return (false);

	return (true);
}
