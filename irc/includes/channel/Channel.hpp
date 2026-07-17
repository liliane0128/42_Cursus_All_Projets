/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 12:38:41 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 13:26:43 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include "utils/IRCNumericCode.hpp"
#include "client/Client.hpp"
#include "server/Server.hpp"

// Channel-wide modes (e.g., +i, +t, +k, +l)
enum ChannelMode
{
	MODE_NONE	= 0,		// No mode
	MODE_INVITE	= (1 << 0), // +i: Invite-only channel
	MODE_TOPIC	= (1 << 1), // +t: Topic protection (only ops can change topic)
	MODE_KEY	= (1 << 2), // +k: Channel key (password)
	MODE_LIMIT	= (1 << 3), // +l: User limit
};

// User-specific modes (e.g., +o)
enum UserMode
{
	USER_MODE_NONE		= 0,		// No mode
	USER_MODE_OPERATOR	= (1 << 0),	// +o: Channel operator 
};

class Channel
{
	public:
		// Constructor
		Channel(const std::string& name);

		// Channel mode management
		void			setMode(ChannelMode mode, bool enable);
		bool			hasMode(ChannelMode mode) const;
		NumericCode		parseModeString(const std::string& modeString,
										const std::string& modeParams);

		// User mode management
		void			setUserMode(Client& client, UserMode mode, bool enable);
		bool			hasUserMode(const Client& client, UserMode mode) const;
		bool			isOperator(const Client& client) const;
		NumericCode		parseUserModeString(const std::string& userModeString,
									Client& client);
		
		// Topic management
		void			setTopic(const std::string& topic);
		const std::string& 	getTopic() const;

		// Name management
		void			setName(const std::string& name);
		const std::string&	getName() const;

		// Client management
		void			addClient(Client& client);
		void			rmClient(Client& client);
		bool			hasClient(const Client& client) const;
		const std::vector<Client*>&	getClients() const;

		// Channel key (password) management
		void			setKey(const std::string& key);
		bool			hasKey() const;
		const std::string&	getKey() const;

		// Invitation management
		void			inviteClient(const Client& client);
		void			revokeInvitation(const Client& client);
		bool			isInvited(const Client& client) const;
		bool			hasInvitation(const std::string& nick) const;

		// User limit management
		void			setUserLimit(size_t limit);
		size_t			getUserLimit() const;
		bool			hasUserLimit() const;

		// Broadcast in the channel
		void			broadcast(const std::string& message); // all
		void			broadcast(const std::string& message,
							const Client* sender);	// all except sender

		// Helper function to validate channel names according to RFC 2812
		static bool		isValidChannelName(const std::string& name);

	private:
		std::string				_name;		// Channel name (e.g., "#general")
		std::string				_topic;		// Channel topic
		int						_modes;		// Bitmask of ChannelMode flags
		std::vector<Client*>	_clients; 	// List of clients in the channel
		std::map<Client*, int>	_userModes;	// Key: Client*, Value: bitmask of UserMode flags
		std::string				_key;		// Channel key (password), if MODE_KEY is set
		size_t					_userLimit;	// User limit, if MODE_LIMIT is set
		std::set<std::string>	_invitedNicks;  // Track invited nicknames
};

#endif
