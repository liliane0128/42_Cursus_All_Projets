/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:39:08 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/19 12:03:47 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
# define FIXED_HPP

#include <iostream>
#include <string>
#include <cmath>

class Fixed
{
private:
	int					_value;
	static const int	_bits = 8;

public:
	Fixed();
	Fixed( int const value );
	Fixed( float const value );
	
	Fixed( const Fixed &other );
	Fixed &operator=( const Fixed &other );
	~Fixed();

	int		getRawBits( void ) const;
	void	setRawBits( int const raw );
	
	
	int toInt( void ) const;
	float toFloat( void ) const;
};

std::ostream& operator<<( std::ostream& output, Fixed const& i );

#endif