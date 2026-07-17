/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LISTCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:57:33 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/03 12:37:27 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTCOMMAND_HPP
#define LISTCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class LISTCommand : public CommandHandler
{
	public:
		LISTCommand();
		virtual ~LISTCommand();

		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
							Server& server);

};

#endif