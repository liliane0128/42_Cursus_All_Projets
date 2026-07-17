/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCReplyBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 12:58:47 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 14:35:41 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCREPLYBUILDER_HPP
#define IRCREPLYBUILDER_HPP

#include "utils/IRCNumericCode.hpp"
#include <string>
#include <map>
#include <vector>

class Server;
class Client;

class IRCReplyBuilder
{
	public:
		IRCReplyBuilder();
		~IRCReplyBuilder();
		
		// Send a numeric reply/error to a client
		void	sendNumericReply(int clientSocket,
									   int code,
									   const std::string& target,
									   const std::vector<std::string>& args,
									   const Server& server) const;

	private:
		// Map of message templates for numeric codes
		typedef std::map<int, std::string>	NumericCodeStringMap;
		NumericCodeStringMap				NumericCodeTemplates;

		// Init the NumericCodeTemplates map
		void		initNumericCodeTemplates();

		// Replaces {0}, {1}, {2}... of a template with the provided arguments
		std::string applyTemplate(const std::string& tmpl,
									const std::vector<std::string>& args) const;

		// Format a numeric reply/error message
		std::string formatNumericReply(int code, const std::string& target,
									const std::string& message) const;
};

#endif
