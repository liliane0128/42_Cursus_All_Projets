/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOISCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:52:28 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 09:39:36 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/WHOISCommand.hpp"

WHOISCommand::WHOISCommand() {}
WHOISCommand::~WHOISCommand() {}

NumericCode WHOISCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}
	
					
void WHOISCommand::execute(int fd, 
						const std::vector<std::string>&params, Server& server)
{
	if (params.empty() || params[0].empty())
	{
		server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, "WHOIS");
		return ;
	}
	
	Client* client = server.getClient(fd);
	if (!client) 
		return ;
	
	std::string myNick = client->getNickname();
	
	Client *target = server.findClientByNick(params[0]);
	if (!target)
	{
		server.sendNumericMsg(fd, ERR_NOSUCHNICK, params[0]);
		return ;
	}
	const std::vector<Channel*>& joinedchannels = client->getJoinedChannels();
	std::string channelList = "";

	for (size_t i = 0; i < joinedchannels.size(); i++)
	{
		if (i > 0)
			channelList += " ";
		if (joinedchannels[i])
			channelList += joinedchannels[i]->getName();
	}
	server.sendReply(fd, ":" + server.getName() + " 311 " + myNick + " "
		+ target->getNickname() + " " + target->getUsername() + " " 
		+ target->getHostname() + " * :" + target->getRealname());
		
	server.sendReply(fd, ":" + server.getName() + " 312 " + myNick + " "
		+ target->getNickname() + " " + server.getName() + " :42 IRC Server");
		
	server.sendReply(fd, ":" + server.getName() + " 319 " + myNick + " "
		+ target->getNickname() + " :" + channelList);
		
	server.sendReply(fd, ":" + server.getName() + " 318 " + myNick + " "
		+ target->getNickname() + " :End of WHOIS list");
		
	return ;
}