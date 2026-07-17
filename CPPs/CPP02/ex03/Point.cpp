/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 13:04:03 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/21 15:05:42 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

Point::Point():_x(0), _y(0) {}

Point::Point( float const x1, float const y1  ):
		_x( x1 ),
		_y( y1 ) {}

Point::~Point() {}

/*
** Point Copy Constructor and Assignment
** 1. We MUST use the Initialization List in the copy constructor 
** because '_x' and '_y' are const and cannot be assigned later.
** 2. The assignment operator cannot modify const members, so it 
** effectively does nothing but return *this.
*/
Point::Point( const Point& other) : _x(other._x), _y(other._y) {}

Point& Point::operator=( const Point& other )
{
	(void)other;
	return *this;
}

Fixed	Point::getX() const
{
	return this->_x;
}

Fixed	Point::getY() const
{
	return this->_y;
}