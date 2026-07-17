/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:46:04 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/12 12:06:21 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "utils/IRCReplyBuilder.hpp"
#include "commands/CommandParser.hpp"
#include "client/Client.hpp"
#include "channel/Channel.hpp"
#include <sys/socket.h>	// socket(), close(), setsockopt(), getsockname(), 
						// bind(), connect(), listen(), accept(), send(), recv()
#include <netinet/in.h>	// htons(), htonl(), ntohs(), ntohl()
#include <arpa/inet.h>	// inet_addr(), inet_ntoa(), inet_ntop(), inet_pton()
#include <unistd.h>		// close(), lseek(), fstat(), fcntl()
#include <poll.h>		// poll(), struct pollfd
#include <vector>		// std::vector
#include <map>			// std::map
#include <set>			// std::set
#include <string>		// std::string
#include <iostream>		// std::cout, std::cerr
#include <cerrno>		// errno
#include <cstring>		// strerror(), memset(), memcpy()
#include <utility>		// std::make_pair
#include <csignal>		// signal(), sigaction(), sigemptyset(), sigfillset(),
						// sigaddset(), sigdelset(), sigismember()
#include <fcntl.h>

#define MAX_MSG_SIZE	1024
#define TIMEOUT_LIMIT	300 // 5 minutes

enum e_status
{
	SUCCESS = 0,
	NO_DATA = 1,
	CLIENT_REMOVED = 2
};

class Server
{
	private:
		std::string							_servername;
		int									_port;
		std::string							_password;
		bool								_running;
		int									_server_fd;
		std::string							_creationTime;
		std::vector<struct pollfd>			_fds;
		std::map<int, Client>				_clients;
		std::map<std::string, std::string>	_operators;
		std::map<std::string, Channel>		_channels;
		std::map<std::string, int>			_NickToFd;
		std::set<int>						_fdsToRemove;
		IRCReplyBuilder						_replyBuilder;
		CommandParser						_commandParser;
		static volatile sig_atomic_t		_runningSignal;
		// static:		Allows access without a class instance.
 		// volatile:	Prevents compiler optimization, 
		// 				ensuring the most recent value is read from memory.
 		// sig_atomic_t:	An integer type that can be accessed as an atomic
		//					entity (i.e. stable when being read) even in the
		//					presence of signals.

		// Client management
		void		acceptNewClient();
		void		removeClient(int fd);

		// Command management
		e_status	handleClientMessage(int fd);
		void		executeCommand(int fd, const std::string &cmdLine);

		// Signal management
		static void	handleSignal(int sig);


	public:
		Server(int port, const std::string &password);
		~Server();

		// Server lifecycle management
		void		start();
		void		stop();
		void		run();
		bool		isRunning() const;
		void		setRunning(bool state);

		// Network configuration
		int			getPort() const;
		void		setPort(int port);
		std::string	getPassword() const;
		void		setPassword(std::string password);

		// Client management
		std::map<int, Client>	getAllClients();
		bool		hasClient(int clientFD) const;
		Client*		getClient(int fd);
		Client*		findClientByNick(const std::string& nick);
		void		markForDeletion(int fd);
		void		checkRegisteration(int fd);

		// Nickname management
		std::map<std::string, int>	getNickToFd();
		void		addNick(const std::string& nick, int fd);
		void		removeNick(const std::string& nick);
		int			getFdByNick(const std::string& nick) const;
		bool		isNicknameTaken(const std::string& nick) const;
		void		broadcastNicknameChange(Client& client, const std::string& msg);

		// Channel management
		Channel*	getChannel(const std::string& name);
		void		createChannel(const std::string& name);
		void		destroyChannel(const std::string& name);
		bool		channelExists(const std::string& name) const;
		Channel*	findChannel(const std::string& name);
		const std::map<std::string, Channel>&		getChannels() const;

		// Operator management
		const std::map<std::string, std::string>&	getOperators() const;

		// Communication methods
		void		sendReply(int fd, std::string msg);
		void		sendNumericMsg(int fd, int code,
									const std::string& arg1 = "",
									const std::string& arg2 = "",
									const std::string& arg3 = "",
									const std::string& arg4 = "",
									const std::string& arg5 = "",
									const std::string& arg6 = "",
									const std::string& arg7 = "",
									const std::string& arg8 = "");

		// Server identification
		std::string	getName();

};

#endif
