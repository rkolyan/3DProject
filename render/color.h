#ifndef COLOR_H
#define COLOR_H

struct Color
{
	double red;
	double green;
	double blue;

	Color(double _red, double _green, double _blue, double _luminosity = 1.0);
	Color();
	Color &operator+=(const Color &other);
	Color &operator*=(const Color &other);
	Color &operator*=(double factor);
	Color &operator/=(double denom);
};

inline Color operator*(const Color& color1, const Color& color2)
{
    return Color(color1.red * color2.red, color1.green * color2.green, color1.blue * color2.blue);
}

inline Color operator*(double scalar, const Color &color)
{
    return Color(scalar * color.red, scalar * color.green, scalar * color.blue);
}

inline Color operator+(const Color& color1, const Color& color2)
{
    return Color(color1.red + color2.red, color1.green + color2.green, color1.blue  + color2.blue);
}

#endif //COLOR_H
