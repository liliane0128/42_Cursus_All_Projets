/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: neandrie <neandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 17:13:23 by lilwang           #+#    #+#             */
/*   Updated: 2026/04/10 19:52:07 by neandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bot/Bot.hpp"
#include "client/Client.hpp"
#include "channel/Channel.hpp"

volatile sig_atomic_t Bot::_runningSignal = true;

Bot::Bot(const char *nickname) : _fd(-1), _nickname(nickname)
{
	std::srand(std::time(NULL));
}

Bot::~Bot()
{
	_runningSignal = false;
	if (_fd >= 0)
		close(_fd);
	std::cout << "Bot destroyed" << std::endl;
}

int		Bot::getFd() const
{
	return _fd;
}

void	Bot::setFd(int fd)
{
	_fd = fd;
}

void	Bot::handleSignal(int sig)
{
	(void)sig;
	
	_runningSignal = false;
}

void	Bot::sendMessage(const std::string &msg)
{
	std::string send_msg = msg;
	if (send_msg.empty() || send_msg.size() < 2 || send_msg.substr(send_msg.size() - 2) != "\r\n")
		send_msg += "\r\n";

	std::cout << "[Bot Send] " << send_msg;
	
	ssize_t total_sent = 0;
	ssize_t msg_len = send_msg.length();
	while (total_sent < msg_len)
	{
		ssize_t sent = send(_fd, send_msg.c_str() + total_sent, msg_len - total_sent, 0);
		if (sent == -1)
		{
			std::cerr << "Error: Failed to send to server" << std::endl;
			break;
		}
		total_sent += sent;
	}
}

// Handle PING messages from server
void	Bot::handlePing(const std::string &buf)
{
	if (buf.size() >= 4 && buf.substr(0, 4) == "PING")
	{
		std::string token;
		if (buf.size() > 5)
			 token = buf.substr(5); // token after "PING"
		std::string pong = "PONG " + token;
		sendMessage(pong);
	}
}

// Wait until bot is registered (receives "001")
void	Bot::waitRegistration()
{
	char raw_buf[BUF_SIZE];
	int bytes_read;

	while (1)
	{
		bytes_read = recv(_fd, raw_buf, BUF_SIZE - 1, 0);
   		if (bytes_read > 0 ) {
			raw_buf[bytes_read] = '\0';
   			std::cout << "[Bot Received] " << raw_buf << std::endl;
		}
		if (bytes_read <= 0)
		{
			usleep(1000); // sleep 1ms to reduce CPU usage
			continue;
		}
		raw_buf[bytes_read] = '\0';
		std::string buf(raw_buf);
		handlePing(buf);

		if (buf.find("001") != std::string::npos) // registration complete
			  break;
	}
}
 
void	Bot::registerBot()
{
	sendMessage("NICK ChatBot");
	sendMessage("USER bot 0 * :My IRC Bot");
	waitRegistration();
}

void	Bot::runLoop()
{
	char raw_buf[BUF_SIZE];
	memset(raw_buf, 0, BUF_SIZE);
	
	while (_runningSignal)
	{
		ssize_t bytes_read = recv(_fd, raw_buf, BUF_SIZE - 1, 0);
		if (bytes_read == 0)
		{
			std::cout << "Server closed connection, stopping bot." << std::endl;
			_runningSignal = false;
			break;
		}
		else if (bytes_read < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
	   			continue;
			std::cerr << "Recv error or server disconnected, stopping bot. errno=" << errno << std::endl;
			_runningSignal = false;
	   		break;
		}
		
		raw_buf[bytes_read] = '\0';
		std::string buf(raw_buf);
		handlePing(buf);
		std::string sender = "";
		if (buf[0] == ':')
		{
			size_t exclamationPos = buf.find('!');
			if (exclamationPos != std::string::npos)
				sender = buf.substr(1, exclamationPos - 1);
		}
		size_t	privmsgPos = buf.find("PRIVMSG");
		if (privmsgPos != std::string::npos)
		{
			size_t targetStart = privmsgPos + 8;
			size_t targetEnd = buf.find(" ", targetStart);
			if (targetEnd == std::string::npos) return;
			std::string target = buf.substr(targetStart, targetEnd - targetStart);

			size_t msgStart = buf.find(":", targetEnd);
			if (msgStart != std::string::npos)
			{
				std::string text = buf.substr(msgStart + 1);
				if (!text.empty() && text[text.size() - 1] == '\n')
					text.erase(text.size() - 1);
				if (!text.empty() && text[text.size() - 1] == '\r')
					text.erase(text.size() - 1);
					
				std::string actualReplyTarget = target;
				if  (target == _nickname)
					actualReplyTarget = sender;
				else
					continue;
				processCommand(actualReplyTarget, text);
			}
		}
	}
}

void	Bot::processCommand(const std::string& target, const std::string& text)
{
	std::string response = "";
	
	if (text == "!time")
	{
		std::time_t now = std::time(NULL);
		char time_str[100];
		std::strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
		response = "Current time is: " + std::string(time_str);
	}
	else if (text == "!joke")
	{
		std::vector<std::string> jokes;
		jokes.push_back("Why do programmers prefer dark mode? Because light attracts bugs.");
		jokes.push_back("C and C++ went to a five star bar, C was stopped by the gate guards because C got no class.");
		jokes.push_back("How do robots eat pizza? One byte at a time.");
		jokes.push_back("A programmer's wife tells him : \"While you're at the store, get some milk.\" He never came back.");
		jokes.push_back("There are 10 types of people in the world: those who understand binary, and those who don't.");
		jokes.push_back("Q: How many programmers does it take to change a light bulb? A: None. That's a hardware problem.");
		jokes.push_back("I can tell you a UDP joke, but you might not get it.");
		jokes.push_back("A programmer is at work when his wife calls and asks him to go to the store. "
						"She says she needs a gallon of milk, and if they have fresh eggs, buy a dozen. "
						"He comes home with 12 gallons of milk.");
		jokes.push_back("What do you call 8 Hobbits? A Hobbyte.");
		jokes.push_back("Why do programmers always confuse Halloween and Christmas? Because 31 Oct. = 25 Dec.");
		jokes.push_back("The oldest computer can be traced back to Adam and Eve. "
						"It was an apple but with extremely limited memory. Just 1 byte. "
						"And then everything crashed.");

		response = jokes[std::rand() % jokes.size()];
	}
	else if (text == "!dice")
	{
		int roll = (std::rand() % 6) + 1;
		std::stringstream ss;
		ss << "🎲 You rolled a " << roll << "!";
		response = ss.str();
	}

	if (!response.empty())
	{
		std::string cmd = "PRIVMSG " + target + " :" + response;
		sendMessage(cmd);
	}
}