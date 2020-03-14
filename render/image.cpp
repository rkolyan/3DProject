#include "image.h"

ImageBuffer::ImageBuffer(size_t _pixels_width, size_t _pixels_height, const Color &backgroundColor): pixels_width(_pixels_width), pixels_height(_pixels_height)
{
	array = new PixelData[_pixels_height * _pixels_width];
}

ImageBuffer::~ImageBuffer()
{
	delete[] array;
    array = nullptr;
	pixels_width = pixels_height = 0;
}

PixelData& ImageBuffer::pixel(size_t i, size_t j) const
{
    return array[(i * pixels_width) + j];
}

size_t ImageBuffer::getPixelsWidth() const
{
	return pixels_width;
}

size_t ImageBuffer::getPixelsHeight() const
{
	return pixels_height;
}

double ImageBuffer::maxColorValue() const
{
	double max = 0.0;
	double pixels_count = pixels_width * pixels_height;

	for (size_t i = 0; i < pixels_count; i++) 
	{
		if (array[i].color.red > max)
			max = array[i].color.red;
		if (array[i].color.green > max)
			max = array[i].color.green;
		if (array[i].color.blue > max)
			max = array[i].color.blue;
	}
	if (max == 0.0)
		max = 1.0;
	return max;
}

#define OPAQUE_ALPHA_VALUE 255
#define BYTES_PER_PIXEL 4

std::vector<unsigned char> *ImageBuffer::convertToRaw()
{
	std::vector<unsigned char> *rgba = new std::vector<unsigned char>(pixels_width * pixels_height * BYTES_PER_PIXEL);
	size_t index = 0;

	const double max_color_value = maxColorValue();

	for (size_t i = 0; i < pixels_height; i++)
	{
		for (size_t j = 0; j < pixels_width; j++)
		{
			(*rgba)[index++] = convertPixelValue(pixel(i, j).color.red, max_color_value);
			(*rgba)[index++] = convertPixelValue(pixel(i, j).color.green, max_color_value);
			(*rgba)[index++] = convertPixelValue(pixel(i, j).color.blue, max_color_value);
			(*rgba)[index++] = OPAQUE_ALPHA_VALUE;
		}
	}

	return rgba;
}

#define COLOR_CHANNEL_MAXIMUM 255

unsigned char ImageBuffer::convertPixelValue(double color_component, const double max_color_value)
{
	size_t pixel_value = static_cast<size_t> (COLOR_CHANNEL_MAXIMUM * color_component / max_color_value);

	if (pixel_value < 0)
		pixel_value = 0;
	else if (pixel_value > COLOR_CHANNEL_MAXIMUM)
		pixel_value = COLOR_CHANNEL_MAXIMUM;

	return static_cast<unsigned char>(pixel_value);
}
