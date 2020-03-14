#include "constants.h"
#include "intersection.h"

#include <cmath>

Intersection::Intersection(): distance_squared(1.0e+20), point(), surface_normal(), solid(nullptr), context(nullptr) 
{
}

int pickClosestIntersection(const IntersectionList &list, Intersection &intersection)
{
	const size_t count = list.size();
	switch (count)
	{
	case 0:
		return 0;

	case 1:
		intersection = list[0];
		return 1;

	default:
		IntersectionList::const_iterator iter = list.begin();
		IntersectionList::const_iterator end  = list.end();
		IntersectionList::const_iterator closest = iter;
		int tieCount = 1;
		for (++iter; iter != end; ++iter)
		{
			const double diff = iter->distance_squared - closest->distance_squared;
			if (fabs(diff) < EPSILON)
			{
				++tieCount;
			}
			else if (diff < 0.0)
			{
				tieCount = 1;
				closest = iter;
			}
		}
		intersection = *closest;
		return tieCount;
	}
}
