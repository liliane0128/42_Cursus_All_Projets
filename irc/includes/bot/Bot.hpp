/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 17:11:45 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/09 11:25:26 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>  // for usleep
#include <vector>
#include <sstream> //std::stringstream
#include <cstdlib>  
#include <ctime>
#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <cerrno>

class Client;
class Channel;

#define BUF_SIZE 512

class Bot
{
private:
	int								_fd;
	std::string						_nickname;
	static volatile sig_atomic_t	_runningSignal;

public:
	Bot(const char *nickname);
	~Bot();

	static void	handleSignal(int sig);
	int		getFd() const;
	void	setFd(int fd);
	void	sendMessage(const std::string &msg);
	void	handlePing(const std::string &buf);
	void	waitRegistration();
	void	registerBot();


    void	runLoop();
	void	processCommand(const std::string& target, const std::string& text);
};

#endif