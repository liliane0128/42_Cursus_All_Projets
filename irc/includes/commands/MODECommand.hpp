/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODECommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:55:28 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/03 12:38:52 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODECOMMAND_HPP
#define MODECOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class MODECommand : public CommandHandler
{
	public:
		MODECommand();
		virtual ~MODECommand();

		NumericCode parse(const std::string& prefix,
							const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::string _target;		// Channel or nickname target
		std::string _modeString;	// Mode string (e.g., "+i", "-k", "+o nick")
		std::string _modeParams;	// Additional params (e.g., keys, nicknames)

		void	handleChannelMode(int fd, Client& client, Server& server);
		void	handleUserMode(int fd, Client& client, Server& server);
};

#endif