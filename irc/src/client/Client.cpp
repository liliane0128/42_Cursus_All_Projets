/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:57:52 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/12 12:10:07 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client/Client.hpp"
#include <algorithm>

Client::Client(int fd) : 
					_fd(fd),
					_nickname(""),
					_username(""),
					_realname(""),
					_hostname(""),
					_buffer(""),
					_registered(false),
					_hasPass(false),
					_isOper(false),
					_lastActivity(std::time(NULL)),
					_joinedChannels()
{}

Client::~Client() {}

int		Client::getFd() const
{
	return _fd;
}

Client::Client(const Client& other)
{
	*this = other;
}

Client& Client::operator=(const Client& other)
{
	if (this != &other)
	{
		_fd = other._fd;
		_nickname = other._nickname;
		_username = other._username;
		_realname = other._realname;
		_hostname = other._hostname;
		_buffer = other._buffer;
		_registered = other._registered;
		_hasPass = other._hasPass;
		_isOper = other._isOper;
		_lastActivity = other._lastActivity;
		_joinedChannels = other._joinedChannels;
	}
	return *this;
}

const std::string& Client::getNickname() const 
{
	return _nickname;
}

void	Client::setNickname(const std::string& nick)
{
	_nickname = nick;
}

const std::string&	Client::getUsername() const
{
	return _username;
}
void	Client::setUsername(const std::string& username)
{
	_username = username;
}
const std::string&	Client::getRealname() const
{
	return _realname;
}

void	Client::setRealname(const std::string& realname)
{
	_realname = realname;
}
 
const std::string&	Client::getHostname() const
{
	return _hostname;
}

void	Client::setHostname(const std::string& hostname)
{
	_hostname = hostname;
}

const std::vector<Channel*>& Client::getJoinedChannels() const
{
	return _joinedChannels;
}

void Client::joinChannel(Channel& channel)
{
	if (std::find(_joinedChannels.begin(), _joinedChannels.end(), &channel)
			== _joinedChannels.end())
	{
		_joinedChannels.push_back(&channel);
	}
}

void Client::leaveChannel(Channel& channel)
{
	std::vector<Channel*>::iterator it
		= std::find(_joinedChannels.begin(), _joinedChannels.end(), &channel);
	if (it != _joinedChannels.end())
	{
		_joinedChannels.erase(it);
	}
}

bool	Client::isRegistered() const
{
	return _registered;
}

void 	Client::setRegistered(bool status)
{
	_registered = status;
}

bool	Client::canBeRegistered() const
{
	return (!_registered && _hasPass && !_nickname.empty() && !_username.empty());
}

bool	Client::hasPass() const
{
	return _hasPass;
}

void	Client::setHasPass(bool status)
{
	_hasPass = status;
}

bool	Client::isOper() const
{
	return _isOper;
}

void	Client::setOper(bool status)
{
	_isOper = status;
}

time_t	Client::getLastActivity() const
{
	return _lastActivity;
}

void	Client::updateLastActivity()
{
	_lastActivity = std::time(NULL);
}

void	Client::addToBuffer(const std::string& data)
{
	_buffer += data;
}

// handle partial packet and sticky packet
std::string Client::getNextCommand()
{
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
		return "";

	std::string cmd = _buffer.substr(0, pos);
	if (cmd.length() > 510)
	{
		std::cout << "Command too long, truncating" << std::endl;
		cmd = cmd.substr(0, 510);
	}
	_buffer.erase(0, pos + 2);

	return cmd;
}

std::string Client::getModeString() const
{
	std::string modeStr;
	modeStr = "+";
	if (_isOper)
		modeStr += 'o';
	return modeStr;
}

NumericCode Client::parseModeString(const std::string& modeString)
{
	bool adding = true;

	for (size_t i = 0; i < modeString.size(); i++)
	{
		char c = modeString[i];

		if (c == '+')      { adding = true;  continue; }
		else if (c == '-') { adding = false; continue; }

		// All user modes are unknown (since we don't support any)
		return ERR_UMODEUNKNOWNFLAG;
	}

	return ERR_NONE;
}

size_t	Client::getBufferSize() const
{
	return _buffer.size();
}

std::string	Client::getPrefix() const
{
	std::string nick = _nickname.empty() ? "*" : _nickname;
	std::string user = _username.empty() ? "unknown" : _username;
	std::string host = _hostname.empty() ? "127.0.0.1" : _hostname;
	
	return	nick + "!" + user + "@" + host;
}

void	Client::sendMsg(const std::string& msg)
{
	if (_fd < 0)
		return ;

	if (send(_fd, msg.c_str(), msg.length(), 0) == -1)
		std::cerr << "Error: Failed to send message to FD: "
											<< _fd << std::endl;
}
