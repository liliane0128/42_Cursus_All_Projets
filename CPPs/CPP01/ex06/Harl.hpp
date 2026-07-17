/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:15:39 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:15:40 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HARL_HPP
# define HARL_HPP

#include <iostream>
#include <vector>
#include <string>

#define DEBUG_MSG "I love having extra bacon for my burger."
#define INFO_MSG "I cannot believe adding extra bacon costs more money."
#define WARN_MSG "I think I deserve some extra bacon for free."
#define ERROR_MSG  "This is unacceptable! I want to speak to the manager now."


class Harl
{
private: 
	void	debug( void );
	void	info( void );
	void	warning( void );
	void	error( void );

public:
	void	complain( std::string level );
};

#endif	