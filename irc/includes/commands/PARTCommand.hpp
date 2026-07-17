/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PARTCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:20:02 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/09 11:26:59 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARTCOMMAND_HPP
#define PARTCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include "channel/Channel.hpp"
#include <vector>
#include <string>

class PARTCommand : public CommandHandler
{
	public:
		PARTCommand();
		virtual ~PARTCommand();

		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::vector<std::string>	_channels;	// Channels to part from
		std::string					_reason;	// Optional part reason
};

#endif