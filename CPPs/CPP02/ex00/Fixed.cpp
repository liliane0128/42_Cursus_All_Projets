/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:39:03 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/19 10:49:57 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _value(0)
{
	std::cout << "Default constructor called\n";
}

Fixed::Fixed( const Fixed &other )
{
	std::cout << "Copy constructor called\n";

	*this = other;
}

/**
 * Copy assignment operator.
 * * 1. Performs a self-assignment check by comparing 'this' pointer with the address of 'other'.
 * 2. Copies the raw bits from 'other' to the current instance.
 * 3. Returns a reference to the current object (*this) to allow linked assignments (e.g., a = b = c).
 */

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
	std::cout << "getRawBits member function called\n";
	return this->_value;
}

void Fixed::setRawBits( int const raw )
{
	this->_value = raw;
}
