#include <cmath>

#include "vector.h"

Vector::Vector(): x(0.0), y(0.0), z(0.0)
{
}

Vector::Vector(double _x, double _y, double _z): x(_x), y(_y), z(_z)
{
}

const double Vector::magnitudeSquared() const
{
	return (x*x) + (y*y) + (z*z);
}

const double Vector::magnitude() const
{
	return sqrt(magnitudeSquared());
}

const Vector Vector::unitVector() const
{
	const double mag = magnitude();
	return Vector(x / mag, y / mag, z / mag);
}

Vector &Vector::operator*=(const double factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
	return *this;
}

Vector &Vector::operator+=(const Vector& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}
