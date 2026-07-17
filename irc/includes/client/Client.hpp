/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:57:58 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 14:26:55 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <ctime>
#include <sys/socket.h>
#include <iostream>
#include "utils/IRCNumericCode.hpp"

class Channel;

class Client
{
	private:
		int						_fd;
		std::string 			_nickname;
		std::string 			_username;
		std::string 			_realname;
		std::string 			_hostname;
		std::string				_buffer;
		bool					_registered;
		bool					_hasPass;
		bool					_isOper;
		time_t 					_lastActivity;
		std::vector<Channel*>	_joinedChannels;

	public:
		Client(int fd);
		~Client();
		Client(const Client& other);
		Client& operator=(const Client& other);

		// Socket & File Descriptor management
		int						getFd() const;

		// Nickname management
		const std::string&		getNickname() const;
		void					setNickname(const std::string& nick);

		// Username management
		const std::string&		getUsername() const;
		void					setUsername(const std::string& username);

		// Realname management
		const std::string&		getRealname() const;
		void					setRealname(const std::string& realname);

		// Hostname management
		const std::string&		getHostname() const;
		void					setHostname(const std::string& hostname);

		// Activity tracking
		time_t					getLastActivity() const;
		void					updateLastActivity();

		// Channel management
		const std::vector<Channel*>&	getJoinedChannels() const;
		void					joinChannel(Channel& channel);
		void					leaveChannel(Channel& channel);

		// Registration & Authentication
		bool					isRegistered() const;
		void					setRegistered(bool status);
		bool					canBeRegistered() const;
		bool					hasPass() const;
		void					setHasPass(bool status);

		// Operator status
		bool					isOper() const;
		void					setOper(bool status);

		// Mode management
		std::string				getModeString() const;
		NumericCode				parseModeString(const std::string& modeString);

		// Buffer management
		void					addToBuffer(const std::string& data);
		std::string				getNextCommand();
		size_t					getBufferSize() const;

		// Message utilities
		std::string				getPrefix() const;
		void					sendMsg(const std::string& msg);
};

#endif