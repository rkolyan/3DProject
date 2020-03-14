#include "constants.h"
#include "intersection.h"
#include "light.h"
#include "optics.h"
#include "solid_object.h"
#include "vector.h"

bool SolidObject::contains(const Vector &point) const
{
	if (is_fully_enclosed)
	{
		const Vector direction(0.0, 0.0, 1.0);

		enclosure_list.clear();
		appendAllIntersections(point, direction, enclosure_list);

		int enter_count = 0;	 // number of times we enter the solid
		int exit_count  = 0;	 // number of times we exit the solid

		IntersectionList::const_iterator iter = enclosure_list.begin();
		IntersectionList::const_iterator end  = enclosure_list.end();
		for (; iter != end; iter++)
		{
			const Intersection &intersection = *iter;
			const double dotprod = dot(direction, intersection.surface_normal);

			if (dotprod > EPSILON)
				exit_count++;
			else if (dotprod < -EPSILON)
				enter_count++;
		}

		switch (exit_count - enter_count)
		{
		case 0:
			return false;

		case 1:
			return true;
		}
	}
	else
	{
		return false;
	}
	return false;
}

SolidObject::SolidObject(const Vector &_center, bool _is_fully_enclosed): center(_center), refractive_index(REFRACTION_GLASS), is_fully_enclosed(_is_fully_enclosed)
{
}

SolidObject::~SolidObject()
{
}

int SolidObject::findClosestIntersection(const Vector &vantage, const Vector &direction, Intersection &intersection) const
{
	cached_intersection_list.clear();
	appendAllIntersections(vantage, direction, cached_intersection_list);
	return pickClosestIntersection(cached_intersection_list, intersection);
}

Optics SolidObject::surfaceOptics(const Vector &surface_point, const void *context) const
{
	return uniform_optics;
}

double SolidObject::getRefractiveIndex() const
{
	return refractive_index;
}

SolidObject &SolidObject::translate(double dx, double dy, double dz)
{
	center.x += dx;
	center.y += dy;
	center.z += dz;
	return *this;
}

SolidObject &SolidObject::move(double cx, double cy, double cz)
{
	translate(cx - center.x, cy - center.y, cz - center.z);
	return *this;
}

SolidObject &SolidObject::move(const Vector &new_center)
{
	move(new_center.x, new_center.y, new_center.z);
	return *this;
}

const Vector &SolidObject::getCenter() const
{
	return center;
}

void SolidObject::setOptics(const Optics &optics)
{
	uniform_optics = optics;
}

void SolidObject::setMatteGlossBalance(double gloss_factor, const Color &raw_matte_color, const Color &raw_gloss_color)
{
	uniform_optics.setMatteGlossBalance(gloss_factor, raw_matte_color, raw_gloss_color);
}

void SolidObject::setFullMatte(const Color &matte_color)
{
	uniform_optics.setMatteGlossBalance(0.0, matte_color, Color(0.0, 0.0, 0.0));  // irrelevant, but mass pass something
}

void SolidObject::setOpacity(const double opacity)
{
	uniform_optics.setOpacity(opacity);
}

void SolidObject::setRefraction(const double refraction)
{
	refractive_index = refraction;
}

const Optics &SolidObject::getOptics() const
{
	return uniform_optics;
}
