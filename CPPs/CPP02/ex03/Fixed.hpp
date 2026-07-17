/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:39:08 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/21 12:30:08 by lilwang          ###   ########.fr       */
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
	
	//6 comparison operators
	bool	operator>( const Fixed& rhs ) const; //rhs: right-hand side
	bool	operator<( const Fixed& rhs ) const; 
	bool	operator>=( const Fixed& rhs ) const; 
	bool	operator<=( const Fixed& rhs ) const; 
	bool	operator==( const Fixed& rhs ) const; 
	bool	operator!=( const Fixed& rhs ) const;

	//4 arthmetic operators
	Fixed	operator+( const Fixed& rhs ) const;
	Fixed	operator-( const Fixed& rhs ) const;
	Fixed	operator*( const Fixed& rhs ) const;
	Fixed	operator/( const Fixed& rhs ) const;

	//4 decrement/increment
	/*
	** Why the 'int' parameter?
	** 1. It is a "dummy" or "placeholder" parameter used to distinguish 
	** the postfix from the prefix;
	** 2. The compiler passes 0 to this parameter automatically.
	** 3. It allows function overloading based on the signature.
	*/
	Fixed& operator++();
	Fixed operator++( int );
	Fixed& operator--();
	Fixed operator--( int );
	
	/*
	** We need both versions to ensure "Const Correctness":
	** 1. Non-const version: Allows modifying the returned object 
	** 	  if the inputs were mutable.
	** 2. Const version: Allows comparing objects that are marked
	**	  as const (read-only).
	*/
	static Fixed& min( Fixed& a, Fixed& b);
	static const Fixed& min( const Fixed& a, const Fixed& b );
	static Fixed& max( Fixed& a, Fixed& b);
	static const Fixed& max( const Fixed& a, const Fixed& b );
};


std::ostream& operator<<( std::ostream& output, Fixed const& i );

#endif