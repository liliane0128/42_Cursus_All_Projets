/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PINGCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:05:21 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 09:38:16 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/PINGCommand.hpp"

PINGCommand::PINGCommand() {}
PINGCommand::~PINGCommand() {}

NumericCode PINGCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}

void PINGCommand::execute(int fd, 
								const std::vector<std::string>&params, Server& server)
{
	if (params.empty() || params[0].empty())
	{
		server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, "PING");
		return ;
	}
	std::string pongReply = ":" + server.getName() + " PONG " + server.getName() + " :" + params[0];
	server.sendReply(fd, pongReply);

	Client* client = server.getClient(fd);
	if (client)
		client->updateLastActivity();
	return ;
}
