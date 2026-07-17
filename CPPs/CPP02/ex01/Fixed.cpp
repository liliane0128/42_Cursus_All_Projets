/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:39:03 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/28 12:19:38 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _value(0)
{
	std::cout << "Default constructor called\n";
}

Fixed::Fixed(int const value):
_value(value << 8)
{
	std::cout << "Int constructor called\n";
}

Fixed::Fixed(float const value):
_value(roundf(value * (1 << 8)))
{
	std::cout << "Float constructor called\n";
}

Fixed::Fixed( const Fixed &other )
{
	std::cout << "Copy constructor called\n";

	*this = other;
}

Fixed &Fixed::operator=( const Fixed &other )
{
	std::cout << "Copy assignment operator called\n";

	if (this != &other)
		this->_value = other.getRawBits();

	return *this;
}

Fixed::~Fixed()
{
	std::cout << "Destructor called\n";
}

int Fixed::getRawBits( void ) const
{
	return this->_value;
}

void Fixed::setRawBits( int const raw )
{
	this->_value = raw;
}

//right shift by 8 bits
int Fixed::toInt( void ) const
{
	return _value >> 8;
}

// Converts the fixed-point value to a floating-point number
// by dividing by 2^8 to restore the fractional part
float Fixed::toFloat( void ) const
{
	return ((float)_value / (1 << 8));
}

// Overloads the insertion operator to output
// the fixed-point number as a floating-point value
std::ostream& operator<<( std::ostream& output, const Fixed& i)
{
	output << i.toFloat();
	return output;
}
