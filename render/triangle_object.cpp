#include <cmath>

#include "algebra.h"
#include "constants.h"
#include "triangle_object.h"

TriangleObject::TriangleObject(const Vector& _center, bool _isFullyEnclosed) : SolidObject(_center, _isFullyEnclosed)
{
}

int TriangleObject::addPoint(unsigned number, double x, double y, double z)
{
	if (number != point_list.size())
		return -1;
	point_list.push_back(Vector(x, y, z));
	return 0;
}

void TriangleObject::addTriangle(int a_point_index, int b_point_index, int c_point_index, const Optics& optics)
{
	triangle_list.push_back(Triangle(a_point_index, b_point_index, c_point_index, optics));
}

void TriangleObject::addQuad(int a_point_index, int b_point_index, int c_point_index, int d_point_index, const Optics& optics)
{
	addTriangle(a_point_index, b_point_index, c_point_index, optics);
	addTriangle(c_point_index, d_point_index, a_point_index, optics);
}

void TriangleObject::appendAllIntersections(const Vector &vantage, const Vector &direction, IntersectionList &intersection_list) const
{
	TriangleList::const_iterator iter = triangle_list.begin();
	TriangleList::const_iterator end  = triangle_list.end();
	for (; iter != end; iter++)
	{
		const Triangle& tri = *iter;
		const Vector& a_point = point_list[tri.a];
		const Vector& b_point = point_list[tri.b];
		const Vector& c_point = point_list[tri.c];
		double t = 0.0, u = 0.0, v = 0.0; 

		if (attemptPlaneIntersection(vantage, direction, a_point, b_point, c_point, t, u, v) ||
			attemptPlaneIntersection(vantage, direction, b_point, c_point, a_point, t, u, v) ||
			attemptPlaneIntersection(vantage, direction, c_point, a_point, b_point, t, u, v))
		{
				const Vector displacement = t * direction;

			Intersection intersection;
			intersection.distance_squared = displacement.magnitudeSquared();
			intersection.point = vantage + displacement;
			intersection.surface_normal = normalVector(tri);
			intersection.solid = dynamic_cast<SolidObject *>(const_cast<TriangleObject *>(this));
			intersection.context = &tri;
			intersection_list.push_back(intersection);
		}
	}
}

Vector TriangleObject::normalVector(const Triangle& triangle) const
{
	const Vector& a = point_list[triangle.a];
	const Vector& b = point_list[triangle.b];
	const Vector& c = point_list[triangle.c];

	return cross(b - a, c - b).unitVector();
}

SolidObject &TriangleObject::translate(double dx, double dy, double dz)
{
	SolidObject::translate(dx, dy, dz);

	PointList::iterator iter = point_list.begin();
	PointList::iterator end  = point_list.end();
	for (; iter != end; ++iter)
	{
		Vector &point = *iter;
		point.x += dx;
		point.y += dy;
		point.z += dz;
	}

	return *this;
}

SolidObject& TriangleObject::rotateX(double angle_in_degrees)
{
	const double angle_in_radians = Algebra::radiansFromDegrees(angle_in_degrees);
	const double a = cos(angle_in_radians);
	const double b = sin(angle_in_radians);
	const Vector center = getCenter();
	PointList::iterator iter = point_list.begin();
	PointList::iterator end  = point_list.end();
	for (; iter != end; iter++)
	{
		Vector& point = *iter;
		const double dy = point.y - center.y;
		const double dz = point.z - center.z;
		point.y = center.y + (a*dy - b*dz);
		point.z = center.z + (a*dz + b*dy);
	}

	return *this;
}

SolidObject& TriangleObject::rotateY(double angle_in_degrees)
{
	const double angle_in_radians = Algebra::radiansFromDegrees(angle_in_degrees);
	const double a = cos(angle_in_radians);
	const double b = sin(angle_in_radians);
	const Vector center = getCenter();

	PointList::iterator iter = point_list.begin();
	PointList::iterator end  = point_list.end();
	for (; iter != end; iter++)
	{
		Vector& point = *iter;
		const double dx = point.x - center.x;
		const double dz = point.z - center.z;
		point.x = center.x + (a * dx + b * dz);
		point.z = center.z + (a * dz - b * dx);
	}

	return *this;
}

SolidObject& TriangleObject::rotateZ(double angle_in_degrees)
{
	const double angle_in_radians = Algebra::radiansFromDegrees(angle_in_degrees);
	const double a = cos(angle_in_radians);
	const double b = sin(angle_in_radians);
	const Vector center = getCenter();

	PointList::iterator iter = point_list.begin();
	PointList::iterator end  = point_list.end();
	for (; iter != end; ++iter)
	{
		Vector& point = *iter;
		const double dx = point.x - center.x;
		const double dy = point.y - center.y;
		point.x = center.x + (a * dx - b * dy);
		point.y = center.y + (a * dy + b * dx);
	}

	return *this;
}

Optics TriangleObject::surfaceOptics(const Vector& surface_point, const void *context) const
{
	const Triangle& triangle = *static_cast<const Triangle *>(context);
	return triangle.optics;
}

bool TriangleObject::attemptPlaneIntersection(const Vector &vantage, const Vector &direction, const Vector &A, const Vector &B, const Vector &C, double &t, double &u, double &v)
{
	const Vector E1 = B - A;
	const Vector E2 = C - A;
	const Vector pvec = cross(direction, E2);
	const double det = dot(E1, pvec);
	if (det > -EPSILON && det < EPSILON)
		return false;
	const double inv_det = 1.0 / det;
	const Vector tvec = vantage - A;
	u = dot(tvec, pvec) * inv_det;
	if (u < 0.0 || u > 1.0)
		return false;
	const Vector qvec = cross(tvec, E1);
	v = dot(direction, qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0)
		return false;
	t = dot(E2, qvec) * inv_det;
	if (t < EPSILON)
		return false;
	return true;
}

Vector TriangleObject::getPointFromIndex(int point_index) const
{
	return point_list[point_index];
}
