/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASSCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:13:38 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 09:38:11 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/PASSCommand.hpp"

PASSCommand::PASSCommand() {}
PASSCommand::~PASSCommand() {}

NumericCode PASSCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}
							
void PASSCommand::execute(int fd, const std::vector<std::string>&params, Server& server)
{
	if (params.empty() || params[0].empty())
	{
		server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, "PASS");
		return ;
	}
	Client* client = server.getClient(fd);
	if (!client)
		return ;

	if (client->isRegistered())
	{
		server.sendNumericMsg(fd, ERR_ALREADYREGISTRED);
		return ;
	}
	if (params[0] != server.getPassword())
	{
		server.sendNumericMsg(fd, ERR_PASSWDMISMATCH);
		return ;
	}
	client->setHasPass(true);
	std::cout << "Client (FD: " << fd << ") passed password verification." << std::endl;
	
	return ;
}
