/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPERCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:21:51 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 09:37:56 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/OPERCommand.hpp"

OPERCommand::OPERCommand() {}
OPERCommand::~OPERCommand() {} 

NumericCode OPERCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}

void OPERCommand::execute(int fd, const std::vector<std::string>&params, Server& server)
{
	if (params.size() < 2)
	{
		server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, "OPER");
		return ;
	}
	
	Client* client = server.getClient(fd);
	if (!client)
		return ;
	if (!client->isRegistered())
	{
		server.sendNumericMsg(fd, ERR_NOTREGISTERED);
		return ;
	}
	if (client->isOper())
		return ;
	
	const std::map<std::string, std::string>& operators = server.getOperators();
	std::map<std::string, std::string>::const_iterator it = operators.find(params[0]);
	if (it != operators.end() && it->second == params[1])
	{
		client->setOper(true);
		server.sendNumericMsg(fd, RPL_YOUREOPER, client->getNickname());
		std::cout << client->getNickname() << " (FD " << fd << ") promoted to operator" << std::endl;
		return ;
	}
	else
	{
		server.sendNumericMsg(fd, ERR_PASSWDMISMATCH);
		return ;
	}
}