#ifndef RAYTRACE_HPP_
#define RAYTRACE_HPP_

#include "parser.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "threadsafe_queue.hpp"

struct locAndCol {
	int x;
	int y;
	Color cl;
};

class RayTrace
{
public:
	RayTrace(const Parser& par);
	void castPrimaryRays(const int& numberOfThreads, const int& currentThreadNumber);
	
	Scene scene;
	ThreadSafeQueue<locAndCol> colorList;
	double bMax, gMax, rMax;
	

private:
	void primaryRayBehavior(const Ray& aPrimaryRay, const int& x, const int& y);
	bool doesIntersect(const Ray& aPrimaryRay, double& param, const object& obj);
	Color castShadowRays(const Ray& aPrimaryRay, double& param, const object& obj);
	//bool intersectsOtherObjects(const Ray& aShadowRay, const object& obj);

};

#endif