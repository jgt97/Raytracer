#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "ray.hpp"

#include <string>
#include <vector>

struct light
{
	double intensity;
	Point location;
};

struct object
{
	Point center;

	Color color;

	double lambert, radius;
	std::string type;
	Ray normal;
};

struct Scene
{
	struct {
		Point center;

		double focus;

		Ray normal;

		double resolution[2];
		int size[2];

	}camera;

	std::vector<light> lights;

	std::vector<object> objects;

};

#endif