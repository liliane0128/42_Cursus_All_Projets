/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PINGCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:27:33 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/03 12:23:59 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PINGCOMMAND_HPP
# define PINGCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class PINGCommand : public CommandHandler
{
	public:
		PINGCommand();
		virtual ~PINGCommand();
		
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);

	private:
		std::string _pingToken;  // Token to pong back
};

#endif