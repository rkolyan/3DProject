#include "color.h"

Color::Color(double _red, double _green, double _blue, double _luminosity): red(_luminosity * _red), green(_luminosity * _green), blue(_luminosity * _blue) {}

Color::Color(): red(0.0), green(0.0), blue(0.0) {}

Color &Color::operator+=(const Color& other)
{
	red   += other.red;
	green += other.green;
	blue  += other.blue;
	return *this;
}

Color &Color::operator*=(const Color& other)
{
	red   *= other.red;
	green *= other.green;
	blue  *= other.blue;
	return *this;
}

Color &Color::operator*=(double factor)
{
	red   *= factor;
	green *= factor;
	blue  *= factor;
	return *this;
}

Color &Color::operator/=(double denom)
{
	red   /= denom;
	green /= denom;
	blue  /= denom;
	return *this;
}
