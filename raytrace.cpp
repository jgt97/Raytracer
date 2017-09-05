#include "raytrace.hpp"

#include <iostream>

#include <cmath>

RayTrace::RayTrace(const Parser& par)
{
	scene = par.getParsedScene();
	bMax = 1.0;
	gMax = 1.0;
	rMax = 1.0;
}

void RayTrace::castPrimaryRays(const int& numberOfThreads, const int& currentThreadNumber)
{
	double factor = ((currentThreadNumber - 1.0) / numberOfThreads);

	double left = -(scene.camera.size[0] / 2) + (scene.camera.size[0]) * factor;
	double right = left + (scene.camera.size[0] / numberOfThreads);
	double bottom = (scene.camera.size[1] / 2);
	double top = -(scene.camera.size[1] / 2);

	for (double i = left; i < right; ++i){
		for (double j = top; j < bottom; ++j){
			Point startingPoint(scene.camera.center.x, scene.camera.center.y, scene.camera.center.z - scene.camera.focus);
			Point pixelPoint(i * scene.camera.resolution[0], j * scene.camera.resolution[1], 0);
			Ray primRay(startingPoint, pixelPoint);
			primRay.normalize();

			primaryRayBehavior(primRay, i + (scene.camera.size[0] / 2), j + (scene.camera.size[1] / 2));
		}
	}
}

void RayTrace::primaryRayBehavior(const Ray& aPrimaryRay, const int& x, const int& y)
{
	Color ret;
	bool atLeastOneIntersect = false;
	double closestParam;
	auto closestObject = scene.objects.begin();	
	closestObject++;
	bool firstTime = true;
	for (auto it = closestObject; it != scene.objects.end(); ++it){
		double parameter;
		bool intersects = doesIntersect(aPrimaryRay, parameter, *it);
		if (intersects){
			atLeastOneIntersect = true;
			if (firstTime) {
				closestParam = parameter;
				closestObject = it;
				firstTime = false;
			}
			else if (parameter < closestParam){
				closestParam = parameter;
				closestObject = it;
			}
		}
	}
	if (atLeastOneIntersect){
		ret = castShadowRays(aPrimaryRay, closestParam, *closestObject);
	}
	locAndCol temp;
	temp.cl = ret;
	temp.x = x;
	temp.y = y;
	colorList.push(temp);
	return;
}

bool RayTrace::doesIntersect(const Ray& aPrimaryRay, double& param, const object& obj)
{
	Ray lineToCentObj(aPrimaryRay.start, obj.center);
	if (obj.type == "sphere") {
		double proj = dotProduct(aPrimaryRay, lineToCentObj);
		double lineFromCent = pow(proj, 2) - dotProduct(lineToCentObj, lineToCentObj) + pow(obj.radius, 2);
		if (lineFromCent < 0.0) {
			return false;
		}
		double t0 = proj - sqrt(lineFromCent);
		double t1 = proj + sqrt(lineFromCent);
		bool val = false;
		if (t0 > 0.001) {
			param = t0;
			val = true;
		}
		if ((t1 > 0.001) && t1 < t0) {
			param = t1;
			val = true;
		}
		return val;
	}
	else if(obj.type == "plane") 
	{
		Ray surfaceNormal;
		surfaceNormal.dX = obj.normal.dX;
		surfaceNormal.dY = obj.normal.dY;
		surfaceNormal.dZ = obj.normal.dZ;

		double paral = dotProduct(aPrimaryRay, surfaceNormal);
		if (paral < -0.001) {
			param = dotProduct(lineToCentObj, surfaceNormal) / paral;
			return param > 0.001;
		}
		return false;
	}
	return false;
}

Color RayTrace::castShadowRays(const Ray& aPrimaryRay, double& param, const object& obj)
{
	Color col;

	Ray surfaceNormal;
	Point intersection(aPrimaryRay.start.x + aPrimaryRay.dX*param, aPrimaryRay.start.y + aPrimaryRay.dY*param, aPrimaryRay.start.z + aPrimaryRay.dZ*param);
	if (obj.type == "sphere") {
		surfaceNormal.setPoints(obj.center, intersection);
	}
	else {
		surfaceNormal.dX = obj.normal.dX;
		surfaceNormal.dY = obj.normal.dY;
		surfaceNormal.dZ = obj.normal.dZ;

	}

	for (auto it = scene.lights.begin(); it != scene.lights.end(); ++it) 
	{
		
		Ray shadowRay(intersection, it->location);
		shadowRay.normalize();

		//bool collides = intersectsOtherObjects(shadowRay, obj);
		bool collides = false;
		if (!collides)
		{
			Color contribution;

			double scale = dotProduct(surfaceNormal, shadowRay);
			if (scale < 0.0) {
				scale = 0.0;
			}

			contribution.r = scale * it->intensity * obj.color.r;
			contribution.g = scale * it->intensity * obj.color.g;
			contribution.b = scale * it->intensity * obj.color.b;
			col.r = col.r + contribution.r;
			col.g = col.g + contribution.g;
			col.b = col.b + contribution.b;
		}
	}
	if (col.b > bMax) {
		bMax = col.b;
	}
	if (col.g > gMax) {
		gMax = col.g;
	}

	if (col.r > rMax) {
		rMax = col.r;
	}
	return col;
}

/*
bool RayTrace::intersectsOtherObjects(const Ray& aShadowRay, const object& obj)
{
	if (obj.type == "sphere") {
		Ray normal(obj.center, aShadowRay.start);
		double d = dotProduct(normal, aShadowRay);
		if (d < 0) {
			return true;
		}
	}

	double closestParam;
	bool intersectsImagePlane = false;
	bool objectIntersect = false;
	auto closestObject = scene.objects.begin();
	bool firstTime = true;
	for (auto it = scene.objects.begin(); it != scene.objects.end(); ++it)
	{
		double parameter;
		bool intersects = doesIntersect(aShadowRay, parameter, *it);
		if (intersects)
		{
			if (it == scene.objects.begin()) {
				intersectsImagePlane = true;
			}
			else {
				objectIntersect = true;
			}

			if (firstTime)
			{
				closestParam = parameter;
				closestObject = it;
				firstTime = false;
			}
			else if (parameter < closestParam)
			{
				return true;
			}
		}
	}
	if (!objectIntersect) {
		return false;
	}
	if (!intersectsImagePlane) {
		return true;
	}
	return (closestObject != scene.objects.begin());
}
*/