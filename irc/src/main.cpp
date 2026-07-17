/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 14:09:22 by neandrie          #+#    #+#             */
/*   Updated: 2026/04/10 18:27:22 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "utils/Colors.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <stdexcept>

void parseServerLaunchArgs(int argc, char **argv, int &port, std::string &password)
{
	if (argc != 3)
		throw std::invalid_argument("Expected exactly 2 arguments.");

	Utils::validatePort(std::string(argv[1]), port);
	password = std::string(argv[2]);
	Utils::validatePassword(password);
}

int main(int argc, char **argv)
{
	int port;
	std::string password;

	try
	{
		parseServerLaunchArgs(argc, argv, port, password);

		std::cout << std::endl;
		std::cout << "   _|  |           _)            \n"
				  << "  |    __|          |   __|  __| \n"
				  << "  __|  |            |  |    (    \n"
				  << "|_|   \\__| |_____||_||_|   \\___| \n"
				  << std::endl;

		std::cout << GREEN
				  << "Processing request to create Server on port : "
				  << port << RESET << std::endl;

		Server server(port, password);
		server.start();
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << RED << "Error: " << e.what() << RESET << std::endl;
		return 1;
	}

	return 0;
}