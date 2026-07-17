/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:13:52 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:13:53 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

#include <iostream> 
#include <string>

class Zombie
{
private:

	std::string	_name;

public:

	Zombie ( const std::string& name );
	~Zombie ();

	void announce ( void );

};

Zombie* newZombie(std::string name);
void 	randomChump(std::string name);

#endif