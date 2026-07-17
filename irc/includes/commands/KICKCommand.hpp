/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICKCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:15:09 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 18:07:12 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICKCOMMAND_HPP
#define KICKCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include "channel/Channel.hpp"
#include <vector>
#include <string>

class KICKCommand : public CommandHandler
{
	public:
		KICKCommand();
		virtual ~KICKCommand();

		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::string					_channel;	// Channel from which to kick
		std::vector<std::string>	_nicknames;	// Nicknames to kick
		std::string					_comment;	// Optional kick comment
};

#endif