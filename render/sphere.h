#ifndef SPHERE_H
#define SPHERE_H

#include "solid_object.h"

class Sphere: public SolidObject
{
public:
	Sphere(const Vector& _center, double _radius);
	virtual void appendAllIntersections(const Vector& vantage, const Vector& direction, IntersectionList& intersection_list) const;
	virtual bool contains(const Vector& point) const;

	virtual SolidObject &rotateX(double angle_in_degrees);
	virtual SolidObject &rotateY(double angle_in_degrees);
	virtual SolidObject &rotateZ(double angle_in_degrees);

private:
	double  radius;
};

#endif //SPHERE_H
