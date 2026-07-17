/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:08 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:09 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Replace.hpp"

static void	err_msg(const std::string& msg)
{
	std::cerr << msg << std::endl;
}

int main(int ac, char **av)
{
	if (ac != 4)
		return (err_msg("Usage: ./sed <filename> <string 1> <string 2>" ), 1);

	std::ifstream file(av[1]);
	if (!file.is_open())
		return (err_msg("Error: Open file failed"),1);

	std::stringstream buffer;	//Create a memory-based stream to act as a temporary buffer
	buffer << file.rdbuf(); 	//Direct the file's internal buffer into the stringstream.
	std::string content = buffer.str();	//Convert the accumulated stream data into a single std::string
	file.close();
	//std::cout << "The content :" << content << std::endl;

	std::string res = replaceAll(content, av[2], av[3]);
	//std::cout << "The result: " << res << std::endl;

	std::string outName = std::string(av[1]) + ".replace";
	std::ofstream outfile(outName.c_str());
	
	if (!outfile.is_open())
		return (err_msg("Error: Create outfile failed"), 1);

	outfile << res; //insert the data on the right into the file stream
	outfile.close();

	return 0;
}
