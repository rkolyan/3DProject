#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "vector.h"

struct Light
{
	Light(const Vector& _location, const Color& _color):location(_location), color(_color) {}

	Vector  location;
	Color   color;
};

#endif
