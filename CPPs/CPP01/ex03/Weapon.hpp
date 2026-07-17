/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:14:46 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/16 17:14:47 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	  WEAPON_HPP
# define  WEAPON_HPP

#include <string>
#include <iostream>

class Weapon
{
private:
	std::string _type;

public:
	Weapon( std::string type );
	~Weapon();

	const std::string&	getType( void ) const;
	void				setType( const std::string& newType );
};

#endif
