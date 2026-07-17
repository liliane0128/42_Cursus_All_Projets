/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 15:46:52 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/01 18:06:22 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "utils/IRCNumericCode.hpp"
#include <vector>
#include <string>

class Server;

class CommandHandler
{
	public:
		virtual ~CommandHandler();
		
		// Parse command parameters
		virtual NumericCode	parse(const std::string& prefix,
								 const std::vector<std::string>& params) = 0;

		// Execute the command
		virtual void	execute(int fd, const std::vector<std::string>&params,
										Server& server) = 0;

		// Combined method to parse and execute the command
		void processCommand(const std::string& prefix, int fd,
										const std::vector<std::string>& params,
										Server& server);

	protected:
		std::string			reconstructTrailingParam(
								const std::vector<std::string>& params,
								size_t startIndex) const;
		bool				hasEnoughParams(
								const std::vector<std::string>& params,
								size_t minParams) const;
		std::vector<std::string>	splitCommaSeparatedList(
										const std::string& list) const;
		void				logCommand(const std::string& prefix,
							const std::string& commandName,
							const std::string& target,
							const std::string& additionalInfo = "") const;
};

#endif