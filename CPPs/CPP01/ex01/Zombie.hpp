/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:05 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:06 by lilwang          ###   ########.fr       */
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

	void setName (const std::string& name );
	~Zombie ();

	void announce ( void );

};

Zombie* zombieHorde( int N, std::string name );


#endif