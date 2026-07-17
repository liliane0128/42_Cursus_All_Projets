/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOINCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:55:57 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/09 15:10:22 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef JOINCOMMAND_HPP
#define JOINCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include "channel/Channel.hpp"
#include <vector>
#include <string>

class JOINCommand : public CommandHandler
{
	public:
		JOINCommand();
		virtual ~JOINCommand();

		NumericCode parse(const std::string& prefix,
							const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::vector<std::string> _channels;	// List of channels to join
		std::vector<std::string> _keys;		// List of keys for the channels

		void sendChannelClientList(int fd, Channel& channel, Server& server);
};

#endif