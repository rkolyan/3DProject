#ifndef SOLID_OBJECT_H
#define SOLID_OBJECT_H

#include <vector>

#include "intersection.h"
#include "optics.h"
#include "vector.h"

class SolidObject
{
public:
	SolidObject(const Vector &_center = Vector(), bool _is_fully_enclosed = true);

	virtual ~SolidObject();
	virtual void appendAllIntersections(const Vector &vantage, const Vector &direction, IntersectionList &intersection_list) const = 0;

	int findClosestIntersection(const Vector &vantage, const Vector &direction, Intersection &intersection) const;
	virtual bool contains(const Vector &point) const;
	virtual Optics surfaceOptics(const Vector &surface_point,const void *context) const;
	double getRefractiveIndex() const;

	virtual SolidObject &rotateX(double angle_in_degrees) = 0;
	virtual SolidObject &rotateY(double angle_in_degrees) = 0;
	virtual SolidObject &rotateZ(double angle_in_degrees) = 0;
	virtual SolidObject &translate(double dx, double dy, double dz);//TODO:ПОПРОБУЙ ВЫВЕСТИ ЭТО ВСЕ В ОТДЕЛЬНЫЙ HPP ФАЙЛ!!!!
	SolidObject &move(double cx, double cy, double cz);
	SolidObject &move(const Vector & new_center);
	const Vector &getCenter() const;

	void setOptics(const Optics &optics);
	void setMatteGlossBalance(double gloss_factor, const Color &raw_matte_color, const Color &raw_gloss_color);
	void setFullMatte(const Color & matte_color);
	void setOpacity(const double opacity);
	void setRefraction(const double refraction);

protected:
	const Optics &getOptics() const;

private:
	Vector center;
	Optics uniform_optics;
	double refractive_index;

	const bool is_fully_enclosed;

	mutable IntersectionList cached_intersection_list;
	mutable IntersectionList enclosure_list;
};

#endif
