/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DCCCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:42:09 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 18:35:41 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/DCCCommand.hpp"
#include <cstdlib>  // For strtoul
#include <sstream>

DCCCommand::DCCCommand() {}
DCCCommand::~DCCCommand() {}

NumericCode DCCCommand::parse(const std::string& prefix,
								const std::vector<std::string>& params)
{
	// Clear previous data to prevent recycling
	_targetNick.clear();
	_filename.clear();
	_IPaddress.clear();
	_filesize = 0;
	_port = 0;
	_rawParams.clear();

	// Check for minimum parameters: DCC SEND filename target-nick [port/ip...]
	if (!hasEnoughParams(params, 4))
		return ERR_NEEDMOREPARAMS;

	// Check if the command is DCC SEND
	if (params[0] != "SEND" && params[0] != "send")
		return ERR_UNKNOWNCOMMAND;

	// Set the filename
	_filename = params[1];

	// Set the target nickname
	_targetNick = params[2];

	// Check if params[3] is an IP address or port number
	bool hasIP = isValidIP(params[3]);

	if (hasIP)
	{
		// IP address format: DCC SEND filename target IP port [filesize]
		if (!hasEnoughParams(params, 5))
			return ERR_NEEDMOREPARAMS;

		_IPaddress = params[3];

		// Try to parse port, but no fail if invalid (DCC is client-to-client)
		_port = static_cast<unsigned int>(safeStoul(params[4]));
		if (_port < 1 || _port > 65535)
			_port = 0; // Set to invalid value, let clients handle it

		if (params.size() >= 6)
		{
			_filesize = safeStoul(params[5]);
		}
	}
	else
	{
		// Standard format: DCC SEND filename target port [filesize]
		_port = static_cast<unsigned int>(safeStoul(params[3]));
		if (_port < 1 || _port > 65535)
			_port = 0; // Set to invalid value, let clients handle it

		if (params.size() >= 5)
		{
			_filesize = safeStoul(params[4]);
		}
	}

	// Store raw parameters for forwarding (starting from index1 to skip "SEND")
	_rawParams = reconstructTrailingParam(params, 1);

	// [DEBUG] Log the DCC SEND attempt
	logCommand(prefix, "DCC SEND attempt", _targetNick, _rawParams);

	return ERR_NONE;
	}

void DCCCommand::execute(int fd, const std::vector<std::string>&params,
								Server& server)
{
	(void)params;
	Client* sender = server.getClient(fd);
	if (!sender)
		return;

	// Find the target client
	Client* target = server.findClientByNick(_targetNick);
	if (!target)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHNICK, _targetNick);
		return;
	}

	// Build DCC message, i.e. forward the raw DCC message as received
	std::string privmsg = ":" + sender->getNickname() + "!" + sender->getUsername() +
						"@" + sender->getHostname() + " PRIVMSG " + _targetNick +
						" :" + _rawParams;

	// Send message to target
	server.sendReply(target->getFd(), privmsg);
}

bool DCCCommand::isValidIP(const std::string& ip) const
{
	// Simple IPv4 validation
	std::string::size_type pos = 0;
	std::string::size_type found;
	int count = 0;
	int num;

	while ((found = ip.find('.', pos)) != std::string::npos)
	{
		if (found - pos < 1 || found - pos > 3) // 1-3 digits per octet
			return (false);

		std::string octet = ip.substr(pos, found - pos);
		num = atoi(octet.c_str());
		if (num < 0 || num > 255)
			return (false);

		pos = found + 1;
		count++;
	}

	// Check last octet
	if (pos >= ip.length() || ip.length() - pos < 1 || ip.length() - pos > 3)
		return (false);

	std::string octet = ip.substr(pos);
	num = atoi(octet.c_str());
	if (num < 0 || num > 255)
		return (false);

	return (count == 3); // Must have 3 dots for 4 octets
}

unsigned long DCCCommand::safeStoul(const std::string& str,
								unsigned long defaultValue) const
{
	if (str.empty())
		return (defaultValue);

	char* endptr;
	unsigned long result = strtoul(str.c_str(), &endptr, 10);

	// Check if conversion failed (non-numeric characters found)
	if (*endptr != '\0')
		return (defaultValue);
	return (result);
}
