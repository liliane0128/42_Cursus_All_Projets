/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:12:04 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/12 17:23:43 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "server/Server.hpp"
#include "utils/Colors.hpp"
#include <iostream> // std::cout, std::cerr
#include <unistd.h> // for usleep()

volatile sig_atomic_t Server::_runningSignal = true;

Server::Server(int port, const std::string &password) : _servername("ircserver"),
														_port(port),
														_password(password),
														_running(false),
														_server_fd(-1),
														_creationTime(""),
														_fds(),
														_clients(),
														_operators(),
														_channels(),
														_NickToFd(),
														_fdsToRemove(),
														_replyBuilder(),
														_commandParser()
{
	_operators["admin"] = "42pass";
	_operators["root"] = "topsecret";
	std::cout << "Server created on port " << _port << std::endl;
}

Server::~Server()
{
	_running = false;
	_runningSignal = false;

	std::cout << std::endl;
	std::cout << "Server being stopped\n";

	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->fd >= 0)
		{
			close(it->fd);
			std::cout << "Closed FD: " << it->fd << std::endl;
		}
	}

	_fds.clear();
	_clients.clear();
	_channels.clear();
	_NickToFd.clear();
	std::cout << "Server destroyed" << std::endl;
}

void Server::handleSignal(int sig)
{
	(void)sig;

	_runningSignal = false;
}

int Server::getPort() const
{
	return (_port);
}

std::string Server::getPassword() const
{
	return (_password);
}

Channel *Server::getChannel(const std::string &name)
{
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return &(it->second);
	return (NULL);
}

bool Server::isRunning() const
{
	return (_running);
}

Client *Server::getClient(int fd)
{
	std::map<int, Client>::iterator it = _clients.find(fd);

	if (it != _clients.end())
		return &(it->second);

	return NULL;
}

std::string Server::getName()
{
	return _servername;
}

const std::map<std::string, Channel> &Server::getChannels() const
{
	return _channels;
}

const std::map<std::string, std::string> &Server::getOperators() const
{
	return _operators;
}

std::map<int, Client> Server::getAllClients()
{
	return _clients;
}

bool Server::hasClient(int clientFD) const
{
	return _clients.find(clientFD) != _clients.end();
}

void Server::setPort(int port)
{
	_port = port;
}

void Server::setPassword(std::string password)
{
	_password = password;
}

void Server::setRunning(bool state)
{
	_running = state;
}

void Server::createChannel(const std::string &name)
{
	if (!_channels.count(name))
		_channels.insert(std::make_pair(name, Channel(name)));
}

void Server::destroyChannel(const std::string &name)
{
	_channels.erase(name); // Destroys the Channel
}

bool Server::channelExists(const std::string &name) const
{
	return (_channels.find(name) != _channels.end());
}

Channel *Server::findChannel(const std::string &name)
{
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return (NULL);
	return &(it->second);
}

/*
 * Initializes the IRC server by creating a non-blocking TCP IPv4 socket,
 * binding it to the specified port, and preparing it to listen for
 * incoming client connections using I/O multiplexing (poll).
 *
 * AF_INET: IPV4 internet protocol
 * SOCK_STREAM: TCP connection
 * 0: default protocol (here TCP)
 * htons: Host to Network Short: convert port number from host byte order to network byte order (big-endian)
 */
void Server::start()
{
	signal(SIGINT, Server::handleSignal);  // Ctrl+C
	signal(SIGTERM, Server::handleSignal); // kill
	signal(SIGQUIT, Server::handleSignal); // Ctrl+\'
	signal(SIGPIPE, SIG_IGN);			   // Ignore SIGPIPE
	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd < 0)
		throw std::runtime_error("fail to create a socket");
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_port);
	serv_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces

	fcntl(_server_fd, F_SETFL, O_NONBLOCK);

	// Enable SO_REUSEADDR to allow the server to re-bind to the port
	// even if it is in TIME_WAIT state from a previous execution.
	int opt = 1; // 1 to enable, 0 to disable
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw std::runtime_error("setsockopt failed");

	// Assign the address and port to the socket
	if (bind(_server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("bind failed");
	}
	// Start listening for incoming connections with a maximum backlog queue
	// SOMAXCONN : Socket Maximum Connections   listen(fd, backlog);
	if (listen(_server_fd, SOMAXCONN) < 0)
	{
		close(_server_fd);
		throw std::runtime_error("Listen failed");
	}

	// Initialize the first pollfd structure for the server itself to monitor new connections
	struct pollfd server_pfd;
	server_pfd.fd = _server_fd;
	server_pfd.events = POLLIN; // Monitor for incoming data/connections
	server_pfd.revents = 0;
	_fds.push_back(server_pfd);

	time_t now = std::time(NULL);
	_creationTime = std::ctime(&now);
	if (!_creationTime.empty() && _creationTime[_creationTime.length() - 1] == '\n')
		_creationTime.erase(_creationTime.length() - 1);
	std::cout << "Server is listening on port " << _port << std::endl;
}

/*
 * The main event loop that monitors all file descriptors for activity.
 * It uses poll() to wait for events without consuming CPU cycles.
 */
void Server::run()
{
	_running = true;

	while (_running && _runningSignal)
	{
		int ret = poll(&_fds[0], _fds.size(), -1);
		if (ret < 0)
		{
			if (errno == EINTR)
				continue; // Interrupted by a signal, not a fatal error
			std::cerr << "Error: poll() failed" << std::endl;
			break;
		}
		std::time_t now = std::time(NULL);
		for (size_t i = 0; i < _fds.size(); ++i)
		{
			if (_fds[i].fd != _server_fd)
            {
				Client* client = getClient(_fds[i].fd);
				if (client && now - client->getLastActivity() > TIMEOUT_LIMIT)
                {
                    std::cout << "Timeout detected for fd: " << _fds[i].fd << std::endl;
                    _fdsToRemove.insert(_fds[i].fd);
                    continue; 
                }
            }
			
			// If no events occurred on this file descriptor, skip to next
			if (_fds[i].revents == 0)
				continue;

			// If the POLLIN bit is set in revents, it means this fd is ready for reading
			// '&' is the bitwise AND operator, used to check specific bits in revents
			if (_fds[i].revents & POLLIN)
			{
				if (_fds[i].fd == _server_fd)
				{
					this->acceptNewClient();
					continue;
				}
				else
				{
					e_status st = this->handleClientMessage(_fds[i].fd);
					if (st == CLIENT_REMOVED)
					{
						_fdsToRemove.insert(_fds[i].fd);
						continue;
					}
					else if (st == NO_DATA)
						continue;
				}
			}
			if (_fds[i].revents & (POLLERR | POLLHUP))
				_fdsToRemove.insert(_fds[i].fd);
		}
		for (std::set<int>::iterator it = _fdsToRemove.begin(); it != _fdsToRemove.end(); ++it)
			removeClient(*it);
		_fdsToRemove.clear();
	}
}

/*
 * Accepts a new incoming connection from the listening socket.
 * Creates a new client socket, sets it to non-blocking, and adds it to the poll list.
 */
void Server::acceptNewClient()
{
	struct sockaddr_in client_addr;

	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);

	if (client_fd < 0)
	{
		std::cerr << "Error: accept failed" << std::endl;
		return;
	}

	struct pollfd client_pfd;
	client_pfd.fd = client_fd;
	client_pfd.events = POLLIN;
	client_pfd.revents = 0;
	_fds.push_back(client_pfd);

	fcntl(client_fd, F_SETFL, O_NONBLOCK);

	_clients.erase(client_fd);
	Client client(client_fd);
	_clients.insert(std::make_pair(client_fd, client));

	std::cout << "New client connected! FD: " << client_fd << std::endl;
}

e_status Server::handleClientMessage(int fd)
{
	char buffer[MAX_MSG_SIZE];
	memset(buffer, 0, MAX_MSG_SIZE);

	ssize_t bytes_read = recv(fd, buffer, MAX_MSG_SIZE, 0);
	if (bytes_read < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return NO_DATA;
		else
		{
			std::cerr << "Error: recv() failed for FD: " << fd
					  << " (" << std::strerror(errno) << ")" << std::endl;
			markForDeletion(fd);
			return CLIENT_REMOVED;
		}
	}
	else if (bytes_read == 0)
	{
		std::cout << "Client (FD: " << fd << ") disconnected." << std::endl;
		markForDeletion(fd);
		return CLIENT_REMOVED;
	}

	Client *client = getClient(fd);
	if (!client)
		return CLIENT_REMOVED;
	client->addToBuffer(std::string(buffer, bytes_read));
	if (client->getBufferSize() > MAX_MSG_SIZE)
	{
		std::cout << "Client (FD: " << fd << ") sent too much data without CRLF. Disconnecting." << std::endl;
		markForDeletion(fd);
		return CLIENT_REMOVED;
	}

	std::string cmd;
	while (!(cmd = client->getNextCommand()).empty())
	{
		this->executeCommand(fd, cmd);
		client->updateLastActivity();
	}
	return SUCCESS;
}


void Server::removeClient(int fd)
{
	Client *client = getClient(fd);
	if (!client)
		return;
	std::vector<Channel*> joinedChannels = client->getJoinedChannels();
	for (size_t i = 0; i < joinedChannels.size(); ++i)
	{
		joinedChannels[i]->broadcast("has been removed");
		joinedChannels[i]->rmClient(*client);
		if (joinedChannels[i]->getClients().empty())
			destroyChannel(joinedChannels[i]->getName());
	}
	std::string nick = client->getNickname();
	removeNick(nick);
	close(fd);
	_clients.erase(fd);
	for (std::vector<struct pollfd>::iterator it = _fds.begin(); it != _fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_fds.erase(it);
			break;
		}
	}
	std::cout << "Client : " << nick
			  << "(FD: " << fd << ") has been removed." << std::endl;
}

void Server::executeCommand(int fd, const std::string &cmdLine)
{
	if (cmdLine.empty())
		return;

	_commandParser.dispatchCommand(fd, cmdLine, *this);
}

void Server::sendNumericMsg(int fd, int code,
							const std::string &arg1, const std::string &arg2,
							const std::string &arg3, const std::string &arg4,
							const std::string &arg5, const std::string &arg6,
							const std::string &arg7, const std::string &arg8)
{
	Client *client = this->getClient(fd);
	if (!client)
		return;
	std::string target = (client && !client->getNickname().empty()) ? client->getNickname() : "*";
	std::vector<std::string> args;
	if (!arg1.empty())
		args.push_back(arg1);
	if (!arg2.empty())
		args.push_back(arg2);
	if (!arg3.empty())
		args.push_back(arg3);
	if (!arg4.empty())
		args.push_back(arg4);
	if (!arg5.empty())
		args.push_back(arg5);
	if (!arg6.empty())
		args.push_back(arg6);
	if (!arg7.empty())
		args.push_back(arg7);
	if (!arg8.empty())
		args.push_back(arg8);

	_replyBuilder.sendNumericReply(fd, code, target, args, *this);
}

void Server::sendReply(int fd, std::string msg)
{
	std::string logMsg = msg;
	if (msg.size() < 2 || msg.substr(msg.size() - 2) != "\r\n")
		msg += "\r\n";

	// TCP is a stream-oriented protocol and has no message boundaries.
	// send() may send only part of the data, so we need to loop until all bytes are sent.
	ssize_t total_sent = 0;
	ssize_t msg_len = msg.length();
	while (total_sent < msg_len)
	{
		ssize_t sent = send(fd, msg.c_str() + total_sent, msg_len - total_sent, 0);
		if (sent == -1)
		{
			std::cerr << "Error: Failed to send message to FD: " << fd << std::endl;
			break;
		}
		total_sent += sent;
	}

	std::cout << "[Send] " << logMsg << std::endl;
}

std::map<std::string, int> Server::getNickToFd()
{
	return _NickToFd;
}

void Server::addNick(const std::string &nick, int fd)
{
	_NickToFd[nick] = fd;
}

void Server::removeNick(const std::string &nick)
{
	_NickToFd.erase(nick);
}

int Server::getFdByNick(const std::string &nick) const
{
	std::map<std::string, int>::const_iterator it = _NickToFd.find(nick);
	if (it == _NickToFd.end())
		return -1;
	return it->second;
}

bool Server::isNicknameTaken(const std::string &nick) const
{
	for (std::map<int, Client>::const_iterator it = _clients.begin();
		 it != _clients.end(); ++it)
	{
		if (it->second.getNickname() == nick)
			return true;
	}
	return false;
}

void Server::checkRegisteration(int fd)
{
	Client *client = getClient(fd);
	if (!client)
		return;

	if (client->isRegistered())
		return;
	if (client->getNickname().empty() || client->getUsername().empty() || !client->hasPass())
		return;

	client->setRegistered(true);
	this->sendReply(fd, ":" + _servername + " 001 " + client->getNickname() + " :Welcome to the Internet Relay Network " + client->getPrefix());
	this->sendReply(fd, ":" + _servername + " 002 " + client->getNickname() + " :Your host is " + _servername + ", running version 42-irc-v1");
	this->sendReply(fd, ":" + _servername + " 003 " + client->getNickname() + " :This server was created " + _creationTime);
	this->sendReply(fd, ":" + _servername + " 004 " + client->getNickname() + " " + _servername + " 42-irc-v1");

	std::cout << "Client (FD: " << fd << ") is registered" << std::endl;
}

Client *Server::findClientByNick(const std::string &nick)
{
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->second.getNickname() == nick)
			return &(it->second);
	}
	return NULL;
}

void Server::markForDeletion(int fd)
{
	_fdsToRemove.insert(fd);
}

void Server::broadcastNicknameChange(Client &client, const std::string &msg)
{
	const std::vector<Channel *> &channels = client.getJoinedChannels();

	for (size_t i = 0; i < channels.size(); i++)
	{
		Channel *channel = channels[i];
		if (!channel)
			continue;

		channel->broadcast(msg);
	}
}
