#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
	double x;
	double y;
	double z;

	Vector();
	Vector(double _x, double _y, double _z);

	const double magnitudeSquared() const;
	const double magnitude() const;
	const Vector unitVector() const;

	Vector& operator *= (const double factor);
	Vector& operator += (const Vector& other);
};

inline Vector operator+(const Vector &a, const Vector &b)
{
    return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector operator-(const Vector &a, const Vector &b)
{
    return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector operator-(const Vector &a)
{
    return Vector(-a.x, -a.y, -a.z);
}

inline double dot(const Vector &a, const Vector &b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline Vector cross(const Vector &a, const Vector &b)
{
    return Vector(
        (a.y * b.z) - (a.z * b.y),
        (a.z * b.x) - (a.x * b.z),
        (a.x * b.y) - (a.y * b.x)
    );
}

inline Vector operator*(const Vector &v, double s)
{
    return Vector(s * v.x, s * v.y, s * v.z);
}

inline Vector operator*(double s, const Vector &v)
{
    return Vector(s * v.x, s * v.y, s * v.z);
}

inline Vector operator/(const Vector &v, double s)
{
    return Vector(v.x / s, v.y / s, v.z / s);
}

#endif
