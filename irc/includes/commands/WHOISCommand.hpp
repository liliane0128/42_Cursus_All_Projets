/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOISCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:43:34 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/01 18:08:12 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WHOISCOMMAND_HPP
# define WHOISCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class WHOISCommand : public CommandHandler
{
	public:
		WHOISCommand();
		virtual ~WHOISCommand();
		
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);
};

#endif