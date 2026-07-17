/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUITCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:27:36 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/03 12:24:25 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUITCOMMAND_HPP
# define QUITCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class QUITCommand : public CommandHandler
{
	public:
		QUITCommand();
		virtual ~QUITCommand();
	
		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);

	private:
		std::string _quitMessage;  // Optional quit message
};

#endif