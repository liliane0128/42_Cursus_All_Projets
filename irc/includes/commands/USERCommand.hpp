/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USERCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:27:38 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/06 18:58:31 by lilwang          ###   ########.fr       */
/*   Updated: 2026/04/03 12:26:18 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERCOMMAND_HPP
# define USERCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class USERCommand : public CommandHandler
{
	public:
		USERCommand();
		virtual ~USERCommand();
		
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);

	private:
		std::string _username;	// Username
		std::string _realname;	// Real name/gecos

		bool	isValidUserFormat(const std::string& user);
};

#endif