/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 12:53:12 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/21 14:08:23 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_HPP
# define POINT_HPP

#include "Fixed.hpp"

class Point
{
private:
	Fixed const	_x;
	Fixed const	_y;

public:
	Point();
	Point( float const x1, float const y1 );
	Point( const Point& other);
	Point& operator=( const Point& other );
	~Point();

	Fixed getX() const;
	Fixed getY() const;
};

#endif