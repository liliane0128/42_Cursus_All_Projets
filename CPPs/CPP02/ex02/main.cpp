#include "Fixed.hpp"

/*
** Why 1 becomes 0.00390625?
** Because in a fixed-point system with 8 fractional bits:
** 1 (raw) = 1 / (2^8) = 1 / 256 = 0.00390625.
** Incrementing the raw bits by 1 is incrementing the value by its smallest precision.
*/
int	main( void )
{
	Fixed			a;
	Fixed const		b( Fixed(5.05f) * Fixed( 2 ));

	std::cout << a << std::endl;
	std::cout << ++a << std::endl;
	std::cout << a << std::endl;
	std::cout << a++ << std::endl;
	std::cout << a << std::endl;

	std::cout << b << std::endl;

	std::cout << Fixed::max( a, b ) << std::endl;

	return 0;
}

// int main( void )
// {
// 	Fixed a(10);
// 	Fixed b(2);

// 	std::cout << b + a << std::endl;
// 	std::cout << b - a << std::endl;
// 	std::cout << b / a << std::endl;
// 	std::cout << b * a << std::endl;

// 	std::cout << Fixed::max(a, b) << std::endl;
// 	std::cout << Fixed::min(a, b) << std::endl;

// 	return 0;
// }
