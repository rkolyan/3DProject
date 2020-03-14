#ifndef IMAGE_BUFFER_H
#define IMAGE_BUFFER_H

#include <cstdlib>
#include <vector>

#include "color.h"

struct PixelData
{
	Color   color;
	bool    is_ambiguous;

	PixelData(): color(), is_ambiguous(false) {}
};

class ImageBuffer
{
public:
	ImageBuffer(size_t _pixels_width, size_t _pixels_height, const Color &background_color);

	virtual ~ImageBuffer();
	PixelData& pixel(size_t i, size_t j) const;

	size_t getPixelsWidth() const;
	size_t getPixelsHeight() const;
	std::vector<unsigned char> *convertToRaw();

private:        
	unsigned char convertPixelValue(double color_component, const double max_color_value);
	double maxColorValue() const;

	size_t pixels_width;
	size_t pixels_height;
	PixelData *array;
};

#endif //IMAGE_BUFFER_H
