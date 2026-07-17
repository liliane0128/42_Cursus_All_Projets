/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITECommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:04:09 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 18:06:44 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITECOMMAND_HPP
#define INVITECOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include "channel/Channel.hpp"
#include <vector>
#include <string>

class INVITECommand : public CommandHandler
{
	public:
		INVITECommand();
		virtual ~INVITECommand();

		NumericCode parse(const std::string& prefix,
							const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::string _nickname;	// Nickname of the user to invite
		std::string _channel;	// Channel to invite the user to
};

#endif