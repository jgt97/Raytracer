#ifndef RAY_HPP_
#define RAY_HPP_

struct Color
{
	Color& operator=(const Color& col);
	double r, g, b;
	Color() {
		r = 0;
		g = 0;
		b = 0;
	}
};

struct Point
{
	double x, y, z;
	Point() {
		x = 0;
		y = 0;
		z = 0;
	}
	Point(const double& argX, const double& argY, const double& argZ) {
		x = argX;
		y = argY;
		z = argZ;
	}
};

struct Ray
{
	Ray() {}

	Ray(const Point& startPt, const Point& endPt);

	void setPoints(const Point& startPt, const Point& endPt);

	void normalize();

	Point start;

	double dX, dY, dZ;
};

double dotProduct(const Ray& op1, const Ray& op2);

#endif