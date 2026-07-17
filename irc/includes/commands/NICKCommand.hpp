/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICKCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:27:31 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/09 11:26:40 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICKCOMMAND_HPP
# define NICKCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class NICKCommand : public CommandHandler
{
	public:
		NICKCommand();
		virtual ~NICKCommand();
		
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);
	
	private:
		std::string	_newNick;

		bool		isValidNickname(const std::string& nick) const;					
};

#endif