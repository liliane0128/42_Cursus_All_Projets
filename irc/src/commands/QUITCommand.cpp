/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUITCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 18:16:09 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/12 14:13:51 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/QUITCommand.hpp"

QUITCommand::QUITCommand() {}
QUITCommand::~QUITCommand() {}

NumericCode QUITCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}
								
void QUITCommand::execute(int fd,
						const std::vector<std::string>&params, Server& server)
{
	Client* client = server.getClient(fd);
	if (!client)	
		return ;
	
	std::string reason = params.empty() ? "Leaving" : params[0];
	std::string quitMsg = ":" + client->getPrefix() + " QUIT :" + reason;

	std::vector<Channel*> joinedChannels = client->getJoinedChannels();
	for (size_t i = 0; i < joinedChannels.size(); ++i)
	{
		joinedChannels[i]->broadcast(quitMsg);
		joinedChannels[i]->rmClient(*client);
		if (joinedChannels[i]->getClients().empty())
			server.destroyChannel(joinedChannels[i]->getName());
	}

	server.sendReply(fd, quitMsg);
	
	std::cout << "[QUIT] Client (FD :" << fd << ") " << client->getNickname() << " has quit." << std::endl;
	
	server.markForDeletion(fd);
	return ;
}