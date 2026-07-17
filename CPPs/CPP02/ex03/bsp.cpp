/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:09:21 by lilwang           #+#    #+#             */
/*   Updated: 2026/01/28 15:08:58 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

/*
** @brief BSP Algorithm (Cross Product Method):
** 1. We treat AB, BC, and CA as vectors forming the boundary.
** 2. For each edge, we calculate the cross product with a vector 
** pointing from the vertex to the test point P.
** 3. If the point P is consistently on the same side of all three vectors 
** (all positive or all negative), it is inside the triangle.
** 4. Points on the edge result in a cross product of 0 and return false.
**
** @brief Cross Product Logic:
** result = x1*y2 - y1*x2
** 1. Positive (> 0): Point is on the left side of the vector.
** 2. Negative (< 0): Point is on the right side of the vector.
** 3. Zero (== 0): Point is collinear (on the line).
** This is why checking if all three results have the same sign 
** tells us if the point is inside the triangle.
*/

static Fixed	check( Point const a, Point const b, Point const p )
{
	// Vector AB = (b.x - a.x, b.y - a.y)
    // Vector AP = (p.x - a.x, p.y - a.y)
    // Cross Product = AB.x * AP.y - AB.y * AP.x
	return (b.getX() - a.getX()) * (p.getY() - a.getY()) -
			(b.getY() - a.getY()) * (p.getX() - a.getX());
}

bool	bsp( Point const a, Point const b, Point const c, Point const point )
{
	Fixed d1 = check(a, b, point);
	Fixed d2 = check(b, c, point);
	Fixed d3 = check(c, a, point);

	if (d1 > 0 && d2 > 0 && d3 > 0)
		return true;
	else if (d1 < 0 && d2 < 0 && d3 < 0)
		return true;
		
	return false;
}
