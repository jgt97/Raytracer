#include "ray.hpp"

#include <cmath>

Ray::Ray(const Point& strtPt, const Point& endPt)
{
	start = strtPt;
	dX = endPt.x - start.x;
	dY = endPt.y - start.y;
	dZ = endPt.z - start.z;
}

void Ray::normalize()
{
	double mag = sqrt(pow(dX, 2) + pow(dY, 2) + pow(dZ, 2));
	dX = dX / mag;
	dY = dY / mag;
	dZ = dZ / mag;
}

void Ray::setPoints(const Point& startPt, const Point& endPt)
{
	start = startPt;
	dX = endPt.x - start.x;
	dY = endPt.y - start.y;
	dZ = endPt.z - start.z;
}

double dotProduct(const Ray& op1, const Ray& op2)
{
	return op1.dX*op2.dX + op1.dY*op2.dY + op1.dZ*op2.dZ;
}

Color& Color::operator=(const Color& col)
{
	r = col.r;
	g = col.g;
	b = col.b;
	return *this;
}
