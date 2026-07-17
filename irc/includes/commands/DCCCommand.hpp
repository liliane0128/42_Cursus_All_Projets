/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DCCCommand.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 17:37:44 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 18:28:21 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DCCCOMMAND_HPP
#define DCCCOMMAND_HPP

#include "commands/CommandHandler.hpp"
#include "server/Server.hpp"
#include "utils/IRCReplyBuilder.hpp"
#include <vector>
#include <string>

class DCCCommand : public CommandHandler
{
	public:
		DCCCommand();
		virtual ~DCCCommand();

		NumericCode parse(const std::string& prefix,
						 const std::vector<std::string>& params);
		void		execute(int fd, const std::vector<std::string>&params,
						Server& server);

	private:
		std::string		_targetNick;	// Target nickname for DCC
		std::string		_filename;		// Filename for file transfers
		std::string		_IPaddress;		// IP address for direct connection
		unsigned long	_filesize;		// File size in bytes
		unsigned int	_port;			// Port number for connection
		std::string		_rawParams;		// Raw parameters for forwarding

		bool			isValidIP(const std::string& ip) const;
		unsigned long	safeStoul(const std::string& str,
							unsigned long defaultValue = 0) const;
};

#endif