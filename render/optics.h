#ifndef OPTICS_H
#define OPTICS_H

#include "color.h"

class Optics
{
public:
	Optics();
	explicit Optics(Color _matte_color, Color _gloss_color = Color(0.0, 0.0, 0.0), double _opacity = 1.0);

	void setMatteGlossBalance(double gloss_factor, const Color& raw_matte_color, const Color& raw_gloss_color);// 0..1: balance between matte and gloss

	void setMatteColor(const Color& _matte_color);
	void setGlossColor(const Color& _gloss_color);
	void setOpacity(double _opacity);

	const Color& getMatteColor() const;
	const Color& getGlossColor() const;
	const double getOpacity()    const;

private:
	Color   matte_color;
	Color   gloss_color;
	double  opacity;
};

#endif // OPTICS_H
