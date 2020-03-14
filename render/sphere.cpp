#include "constants.h"
#include "sphere.h"

#include <cmath>

void Sphere::appendAllIntersections(const Vector& vantage, const Vector& direction, IntersectionList& intersection_list) const
{
	const Vector displacement = vantage - getCenter();
	const double a = direction.magnitudeSquared();
	const double b = 2.0 * dot(direction, displacement);
	const double c = displacement.magnitudeSquared() - radius*radius;
	const double radicand = b*b - 4.0*a*c;
	if (radicand >= 0.0)
	{
		const double root = sqrt(radicand);
		const double denom = 2.0 * a;
        const double u[2] = {(-b + root) / denom, (-b - root) / denom};

        for (int i=0; i < 2; i++)
		{
			if (u[i] > EPSILON)
			{
				Intersection intersection;
				const Vector vantage_to_surface = u[i] * direction;
				intersection.point = vantage + vantage_to_surface;
				intersection.surface_normal = (intersection.point - getCenter()).unitVector();
				intersection.distance_squared = vantage_to_surface.magnitudeSquared();
				intersection.solid = this;
				intersection_list.push_back(intersection);
			}
		}
	}
}

Sphere::Sphere(const Vector &_center, double _radius): SolidObject(_center), radius(_radius)
{
}

bool Sphere::contains(const Vector &point) const
{
	const double r = radius + EPSILON;  
	return (point - getCenter()).magnitudeSquared() <= (r * r);
}

SolidObject &Sphere::rotateX(double angle_in_degrees)
{
	return *this;
}

SolidObject &Sphere::rotateY(double angle_in_degrees)
{
	return *this;
}

SolidObject &Sphere::rotateZ(double angle_in_degrees)
{
	return *this;
}
