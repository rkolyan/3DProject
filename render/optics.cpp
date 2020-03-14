#include <cmath>

#include "optics.h"

Optics::Optics(): matte_color(Color(1.0, 1.0, 1.0)), gloss_color(Color(0.0, 0.0, 0.0)), opacity(1.0)
{
}

Optics::Optics(Color _matte_color, Color _gloss_color, double _opacity)
{
	setMatteColor(_matte_color);
	setGlossColor(_gloss_color);
	setOpacity(_opacity);
}

void Optics::setMatteColor(const Color &_matte_color)
{
	matte_color = _matte_color;
}

void Optics::setGlossColor(const Color &_gloss_color)
{
	gloss_color = _gloss_color;
}

void Optics::setMatteGlossBalance(double gloss_factor, const Color &raw_matte_color, const Color &raw_gloss_color)
{
    //Здесь должны быть коды для проверки корректности
	setMatteColor((1.0 - gloss_factor) * raw_matte_color);
	setGlossColor(gloss_factor * raw_gloss_color);
}

void Optics::setOpacity(double _opacity)
{
	opacity = _opacity;
}

const Color &Optics::getMatteColor() const
{
	return matte_color;
}

const Color &Optics::getGlossColor() const
{
	return gloss_color;
}

const double Optics::getOpacity() const
{
	return opacity;
}
