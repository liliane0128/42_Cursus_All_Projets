/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bot.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 14:39:58 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 18:34:26 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot/Bot.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include "utils/Utils.hpp"

void parseBotArgs(int argc, char **argv, std::string &ip, int &port, std::string &password)
{
    if (argc != 4)
        throw std::invalid_argument("Usage: ./ircbot <ip> <port> <password>");

    ip = argv[1];
    Utils::validatePort(argv[2], port);
    password = argv[3];
    Utils::validatePassword(password);
}

static void initSignals()
{
    signal(SIGINT, Bot::handleSignal);
    signal(SIGTERM, Bot::handleSignal);
    signal(SIGQUIT, Bot::handleSignal);
    signal(SIGPIPE, SIG_IGN);
}

int main(int argc, char **argv)
{
    std::string ip;
    int port;
    std::string password;

	try 
	{ 
		parseBotArgs(argc, argv, ip, port, password);

		Bot bot("ChatBot");
		initSignals();
		int sockFd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockFd < 0)
			throw std::runtime_error("socket failed");
		bot.setFd(sockFd);

		fcntl(sockFd, F_SETFL, O_NONBLOCK);

		struct sockaddr_in serv_addr;
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);
		//Convert IPv4 addresses from text string to binary form suitable for struct in_addr

		int ret = connect(sockFd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

		if (ret < 0)
		{
			if (errno != EINPROGRESS)
				throw std::runtime_error("Connection Failed");
		}

		bot.sendMessage("PASS " + password);

		bot.registerBot();

		std::cout << "Bot is running...\n";

		bot.runLoop();
	}
	
	catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
