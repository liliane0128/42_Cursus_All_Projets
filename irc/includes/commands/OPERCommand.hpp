/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPERCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:20:59 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/03 12:22:52 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERCOMMAND_HPP
# define OPERCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class OPERCommand : public CommandHandler
{
	public:
		OPERCommand();
		virtual ~OPERCommand();
		
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);
						
	private:
		std::string _operatorName; // Operator username
		std::string _operatorPass; // Operator password
};

#endif