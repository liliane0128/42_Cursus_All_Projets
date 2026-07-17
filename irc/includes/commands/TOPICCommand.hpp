/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPICCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/27 15:55:28 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 18:08:01 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOPICCOMMAND_HPP
#define TOPICCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class TOPICCommand : public CommandHandler
{
	public:
		TOPICCommand();
		virtual ~TOPICCommand();

		NumericCode parse(const std::string& prefix,
							const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

	private:
		std::string _channel;
		std::string _topic;
};

#endif