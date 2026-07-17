/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:39:03 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/28 12:23:32 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _value(0) {}
Fixed::~Fixed() {}


Fixed::Fixed(int const value):
		_value(value << 8) {}

Fixed::Fixed(float const value):
		_value(roundf(value * (1 << 8))) {}

Fixed::Fixed( const Fixed &other )
{
	*this = other;
}

Fixed &Fixed::operator=( const Fixed &other )
{
	if (this != &other)
		this->_value = other.getRawBits();

	return *this;
}

int Fixed::getRawBits( void ) const
{
	return this->_value;
}

void Fixed::setRawBits( int const raw )
{
	this->_value = raw;
}


int Fixed::toInt( void ) const
{
	return _value >> 8;
}

float Fixed::toFloat( void ) const
{
	return ((float)_value / (1 << 8));
}

std::ostream& operator<<( std::ostream& output, const Fixed& i)
{
	output << i.toFloat();
	return output;
}

bool	Fixed::operator>( const Fixed& rhs ) const
{
	return (this->_value > rhs._value);
}

bool	Fixed::operator<( const Fixed& rhs ) const
{
	return (this->_value < rhs._value);
}

bool	Fixed::operator>=( const Fixed& rhs ) const
{
	return (this->_value >= rhs._value);
}

bool	Fixed::operator<=( const Fixed& rhs ) const
{
	return (this->_value <= rhs._value);
}

bool	Fixed::operator==( const Fixed& rhs ) const
{
	return (this->_value == rhs._value);
}

bool	Fixed::operator!=( const Fixed& rhs ) const
{
	return (this->_value != rhs._value);
}

Fixed	Fixed::operator+( const Fixed& rhs ) const
{
	Fixed res;
	
	res.setRawBits(this->getRawBits() + rhs.getRawBits());
	return res;
}

Fixed	Fixed::operator-( const Fixed& rhs ) const
{
	Fixed res;
	
	res.setRawBits(this->getRawBits() - rhs.getRawBits());
	return res;
}

/*
** Multiplication operator overload
** Logic: (lhs * rhs) / 2^bits
** 1. Use static_cast<long long> to prevent overflow during the multiplication of two large integers.
** 2. Since multiplying two fixed-point numbers doubles the scaling factor (2^bits * 2^bits),
** we right-shift by _bits to restore the correct fixed-point representation.
*/
Fixed	Fixed::operator*( const Fixed& rhs ) const
{
	Fixed res;

	long long value = (static_cast<long long>(this->getRawBits()) 
			* static_cast<long long>(rhs.getRawBits())) >> _bits; // or / (1 << _bits)
	res.setRawBits (static_cast<int>(value));
	
	return res;
}

/*
** Division operator overload
** 1. Upscale the dividend (this) by shifting left by _bits. 
** This restores the fixed-point scaling factor after division.
** 2. Use long long to prevent overflow during the upscaling.
*/
Fixed	Fixed::operator/( const Fixed& rhs ) const
{
	Fixed res;
	
	long long value = (static_cast<long long>(this->getRawBits())
						<< _bits) / rhs.getRawBits();
	res.setRawBits(static_cast<int>(value));
	
	return res;
}

/*
** Pre-increment operator (++i)
** To increment by epsilon:
** 1. Retrieve the current raw integer value.
** 2. Add 1 to this integer value (representing the smallest step).
** 3. Store the updated value back into the object.
*/
Fixed& Fixed::operator++()
{
	this->setRawBits(this->getRawBits() + 1);
	return *this;
}

/*
** Postfix increment operator (i++)
** Note: Returns by value (Fixed) because we are returning a temporary 
** local object (tmp) that will be destroyed when the function scope ends.
*/
Fixed Fixed::operator++( int )
{
	Fixed tmp(*this);
	
	++(*this);
	return tmp;
}

/*
** Prefix decrement operator (--i)
** Decrements the raw bits by 1. 
** Note: No overflow/underflow check is performed to match the behavior 
** of built-in integer types (wrapping behavior).
*/
Fixed& Fixed::operator--()
{
	this->setRawBits(this->getRawBits() - 1);
	return *this;
}

//Postfix decrement operator (i--)
Fixed Fixed::operator--( int )
{
	Fixed tmp(*this);
	
	--(*this);
	return tmp;
}

/*
** Static min function (non-const)
** Compares two Fixed point objects and returns the smaller one.
** If both are equal, 'b' is returned by convention.
*/
Fixed& Fixed::min( Fixed& a, Fixed& b)
{
	return (a < b) ? a : b;
}

const Fixed& Fixed::min( const Fixed& a, const Fixed& b )
{
	return (a < b) ? a : b;
}

Fixed& Fixed::max( Fixed& a, Fixed& b)
{
	return (a > b) ? a : b;
}

const Fixed& Fixed::max( const Fixed& a, const Fixed& b )
{
	return (a > b) ? a : b;
}
