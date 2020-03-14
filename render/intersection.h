#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>

#include "vector.h"

class SolidObject;

class Intersection
{
public:
	double distance_squared;

	Vector point;
	Vector surface_normal;

    const SolidObject *solid;

	const void* context;

	Intersection();
};

typedef std::vector<Intersection> IntersectionList;

int pickClosestIntersection(const IntersectionList& list, Intersection& intersection);

#endif
