/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USERCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 17:25:31 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 09:38:49 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands/USERCommand.hpp"

USERCommand::USERCommand() {}
USERCommand::~USERCommand() {}

bool	USERCommand::isValidUserFormat(const std::string& user) //reference : RFC 2812 page 8
{
	if (user.empty()) return false;

	for (size_t i = 0; i < user.length(); ++i)
	{
		unsigned char c = static_cast<unsigned char>(user[i]); 
		if (c == '\0' || c == '\r' || c == '\n' || c == ' ' || c == '@') //can  be any octet except NUL, CR, LF, " " and "@"
			return false;
	}
	return true;
}

NumericCode USERCommand::parse(const std::string& prefix, const std::vector<std::string>& params)
{
	(void)prefix;
	(void)params;
	return ERR_NONE;
}

//USER <user> <mode> <unused> <realname>
void USERCommand::execute(int fd,
						const std::vector<std::string>&params, Server& server)
{
	if (params.size() < 4) 
	{
		server.sendNumericMsg(fd, ERR_NEEDMOREPARAMS, "USER");
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
	if (!client->hasPass())
	{
		server.sendNumericMsg(fd,ERR_PASSWDMISMATCH);
		return ;
	}
	if (!isValidUserFormat(params[0]))
		return ;
	client->setUsername(params[0]);
	client->setRealname(params[3]);
	server.checkRegisteration(fd);
	
	return ;
}