#ifndef TRIANGLE_MESH_H
#define TRIANGLE_MESH_H

#include <vector>

#include "intersection.h"
#include "optics.h"
#include "solid_object.h"
#include "vector.h"

struct Triangle
{
	int a;
	int b;
	int c;

	Optics optics;

	Triangle(int _a, int _b, int _c, const Optics& _optics): a(_a), b(_b), c(_c), optics(_optics) {}
};

typedef std::vector<Vector> PointList;
typedef std::vector<Triangle>  TriangleList;

class TriangleObject: public SolidObject
{
public:
	TriangleObject(const Vector& center = Vector(), bool _is_fully_enclosed = true);

	virtual void appendAllIntersections(const Vector& vantage, const Vector& direction, IntersectionList& intersection_list) const;

	virtual SolidObject &translate(double dx, double dy, double dz);
	virtual SolidObject &rotateX(double angle_in_degrees);
	virtual SolidObject &rotateY(double angle_in_degrees);
	virtual SolidObject &rotateZ(double angle_in_degrees);

	int addPoint(unsigned number, double x, double y, double z);
	void addTriangle(int a_point_index, int b_point_index, int c_point_index, const Optics& optics);
	void addQuad(int a_point_index, int b_point_index, int c_point_index, int d_point_index, const Optics& optics);
	virtual Optics surfaceOptics(const Vector& surface_point, const void *context) const;
	static bool attemptPlaneIntersection(const Vector& vantage, const Vector& direction, const Vector& A, const Vector& B, const Vector& C, double &u, double &v, double &w);

	Vector getPointFromIndex(int pointIndex) const;

private:
	Vector normalVector(const Triangle &triangle) const;

	PointList point_list;
	TriangleList triangle_list;
};

#endif
