#include "Point.hpp"

bool bsp( Point const a, Point const b, Point const c, Point const point );

void bsp_test(Point const a, Point const b, Point const c, Point const p)
{
	if (bsp(a,b,c,p))
		std::cout << "Point is inside the triangle\n";
	else
		std::cout << "Point is outside or on the edge of the triangle\n";
}

int main()
{
	Point const a(0, 0);
	Point const b(0, 10);
	Point const c(15, 0);

	bsp_test(a, b, c, Point(5, 5));
	bsp_test(a, b, c, Point(15, 0));
	bsp_test(a, b, c, Point(0.1f, 0.1f));
	bsp_test(a, b, c, Point(20, 20));

	return 0;
}
