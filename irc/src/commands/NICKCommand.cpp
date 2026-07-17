/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICKCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:44:25 by lilwang           #+#    #+#             */      
/*   Updated: 2026/04/06 12:29:29 by lilwang          ###   ########.fr       */                    
/*   Created: 2026/03/25 16:44:25 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/03 16:45:17 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/NICKCommand.hpp"

NICKCommand::NICKCommand() {}
NICKCommand::~NICKCommand() {} 
  
bool	NICKCommand::isValidNickname(const std::string& nick) const //reference : RFC 2812 page 7 
{
	if (nick.empty() || nick.length() > 9)
		return false;
	if (std::isdigit(nick[0]))
		return false;
	std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-[]\\`^{}|_";
	if (nick.find_first_not_of(charset) != std::string::npos)
		return false;

	return true;
}

NumericCode NICKCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}

void NICKCommand::execute(int fd, const std::vector<std::string>&params, Server& server)
{
	Client* client = server.getClient(fd);
	if (!client)
		return ;
	
	if (params.empty() || params[0].empty())
	{
		server.sendNumericMsg(fd, ERR_NONICKNAMEGIVEN);	
		return ;
	}

	if (params.size() > 1)
		return ;
	
	if (!isValidNickname(params[0]))
	{
		server.sendNumericMsg(fd, ERR_ERRONEUSNICKNAME, params[0]);
		return ;
	}
	std::string newNick = params[0];
	if (server.isNicknameTaken(newNick))
	{
		server.sendNumericMsg(fd, ERR_NICKNAMEINUSE, newNick);
		return ;
	}

	// Check for nickname policy violations (e.g., underscore suffix)
	if (newNick.find('_') != std::string::npos &&
		(newNick.substr(newNick.find('_')) == "_"))
	{
		server.sendNumericMsg(fd, ERR_NICKPOLICYVIOLATION, newNick);
		std::cout << "Rejected nickname '" << newNick
				  << "' policy violation (underscore suffix not allowed by server design)" << std::endl;
		return;
	}

	std::string oldNick = client->getNickname();
	if (!oldNick.empty())
		server.removeNick(oldNick);
	client->setNickname(newNick);
	server.addNick(newNick, fd);
	
	if (!oldNick.empty() && oldNick != newNick)
	{
		std::string notify = ":" + oldNick + "!" + client->getUsername() 
							 + "@" + client->getHostname() + " NICK :" + newNick;
		server.broadcastNicknameChange(*client, notify);
		server.sendReply(client->getFd(), notify);
		std::cout << "[NICK]" << notify << std::endl;
		return ;
	}
	
	return ;
}
