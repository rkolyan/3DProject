#include <cmath>
#include <fstream>
#include <iostream>
#include "lodepng.h"

#include "algebra.h"
#include "constants.h"
#include "light.h"
#include "optics.h"
#include "scene.h"
#include "vector.h"

void Scene::clearSolidObjectList()
{
	SolidObjectList::iterator iter = solid_object_list.begin();
	SolidObjectList::iterator end  = solid_object_list.end();
	for (; iter != end; iter++)
	{
		delete *iter;
		*iter = nullptr;
	}
	solid_object_list.clear();
}

Scene::Scene(const Color &_background_color): background_color(_background_color), ambient_refraction(REFRACTION_VACUUM)
{
}

Scene::~Scene()
{
	clearSolidObjectList();
}

SolidObject &Scene::addSolidObject(SolidObject *solidObject)
{
	solid_object_list.push_back(solidObject);
	return *solidObject;
}

void Scene::addLight(const Light &light)
{
	light_list.push_back(light);
}

void Scene::setAmbientRefraction(double refraction)
{
	ambient_refraction = refraction;
}

inline bool isSignificant(const Color &color)
{
	return (color.red   >= MIN_OPTICAL_INTENSITY) ||
		(color.green >= MIN_OPTICAL_INTENSITY) ||
		(color.blue  >= MIN_OPTICAL_INTENSITY);
}

Color Scene::traceRay(const Vector &vantage, const Vector &direction, double refractive_index, Color ray_intensity, int recursion_depth) const
{
	Intersection intersection;
	const int num_closest = findClosestIntersection(vantage, direction, intersection);

	switch (num_closest)
	{
	case 0:
		return ray_intensity * background_color;

	case 1:
		return calculateLighting(intersection, direction, refractive_index, ray_intensity, 1 + recursion_depth);

	default:
		throw AmbiguousIntersectionException();
	}
	return Color(0, 0, 0);
}

Color Scene::calculateLighting(const Intersection &intersection, const Vector &direction, double refractive_index, Color ray_intensity, int recursion_depth) const
{
	Color colorSum(0.0, 0.0, 0.0);

	if (recursion_depth <= MAX_OPTICAL_RECURSION_DEPTH)
	{
		if (isSignificant(ray_intensity))
		{
			const SolidObject& solid = *intersection.solid;
			const Optics optics = solid.surfaceOptics(intersection.point, intersection.context);
			const double opacity = optics.getOpacity();
			const double transparency = 1.0 - opacity;
			if (opacity > 0.0)
			{
				const Color matteColor = opacity * optics.getMatteColor() * ray_intensity * calculateMatte(intersection);
				colorSum += matteColor;
			}

			double refractiveReflectionFactor = 0.0;
			if (transparency > 0.0)
				colorSum += calculateRefraction(intersection, direction, refractive_index, transparency * ray_intensity, recursion_depth, refractiveReflectionFactor);
			Color reflectionColor(1.0, 1.0, 1.0);
			reflectionColor *= transparency * refractiveReflectionFactor;
			reflectionColor += opacity * optics.getGlossColor();
			reflectionColor *= ray_intensity;

			if (isSignificant(reflectionColor))
			{
				const Color matteColor = calculateReflection(intersection, direction, refractive_index, reflectionColor, recursion_depth);
				colorSum += matteColor;
			}
		}
	}

	return colorSum;
}

Color Scene::calculateMatte(const Intersection& intersection) const
{
	Color colorSum(0.0, 0.0, 0.0);

	LightList::const_iterator iter = light_list.begin();
	LightList::const_iterator end  = light_list.end();
	for (; iter != end; iter++)
	{
		const Light &source = *iter;

		if (hasClearLineOfSight(intersection.point, source.location))
		{
			const Vector direction = source.location - intersection.point;
			const double incidence = dot(intersection.surface_normal, direction.unitVector());

			if (incidence > 0.0)
			{
				const double intensity = incidence / direction.magnitudeSquared();
				colorSum += intensity * source.color;
			}
		}
	}

	return colorSum;
}

Color Scene::calculateReflection(const Intersection& intersection, const Vector& incidentDir, double refractive_index, Color ray_intensity, int recursion_depth) const
{
	const Vector& normal = intersection.surface_normal;
	const double perp = 2.0 * dot(incidentDir, normal);
	const Vector reflectDir = incidentDir - (perp * normal);

	return traceRay(intersection.point, reflectDir, refractive_index, ray_intensity, recursion_depth);
}

Color Scene::calculateRefraction(const Intersection &intersection, const Vector &direction, double source_refractive_index, Color ray_intensity, int recursion_depth, double &out_reflection_factor) const
{
	const Vector dirunit = direction.unitVector();

	double cos_a1 = dot(dirunit, intersection.surface_normal);
	double sin_a1;
	if (cos_a1 <= -1.0)
	{
		cos_a1 = -1.0;
		sin_a1 =  0.0;
	}
	else if (cos_a1 >= +1.0)
	{
		cos_a1 = +1.0;
		sin_a1 =  0.0;
	}
	else
	{
		sin_a1 = sqrt(1.0 - cos_a1*cos_a1);
	}

#define SMALL_SHIFT 0.001
	const Vector testPoint = intersection.point + SMALL_SHIFT * dirunit;
	const SolidObject* container = primaryContainer(testPoint);
	const double target_refractive_index = (container != nullptr) ? container->getRefractiveIndex() : ambient_refraction;
	const double ratio = source_refractive_index / target_refractive_index;
	const double sin_a2 = ratio * sin_a1;

	if (sin_a2 <= -1.0 || sin_a2 >= +1.0)
	{
		out_reflection_factor = 1.0;
		return Color(0.0, 0.0, 0.0);
	}

	double k[2] = {0};
	const int numSolutions = Algebra::solveQuadraticEquation(1.0, 2.0 * cos_a1, 1.0 - 1.0/(ratio*ratio), k);
	double maxAlignment = -0.0001;
	Vector refract_dir(0, 0, 0);

	for (int i = 0; i < numSolutions; i++)
	{
		Vector refract_attempt = dirunit + k[i] * intersection.surface_normal;
		double alignment = dot(dirunit, refract_attempt);
		if (alignment > maxAlignment)
		{
			maxAlignment = alignment;
			refract_dir = refract_attempt;
		}
	}

	double cos_a2 = sqrt(1.0 - sin_a2*sin_a2);
	if (cos_a1 < 0.0)
		cos_a2 = -cos_a2;

	const double Rs = polarizedReflection(source_refractive_index, target_refractive_index, cos_a1, cos_a2);
	const double Rp = polarizedReflection(source_refractive_index, target_refractive_index, cos_a2, cos_a1);

	out_reflection_factor = (Rs + Rp) / 2.0;

	const Color next_ray_intensity = (1.0 - out_reflection_factor) * ray_intensity;

	return traceRay(intersection.point, refract_dir, target_refractive_index, next_ray_intensity, recursion_depth);
}

double Scene::polarizedReflection(double n1, double n2, double cos_a1, double cos_a2) const	
{
	const double left  = n1 * cos_a1;
	const double right = n2 * cos_a2;
	double numer = left - right;
	double denom = left + right;
	denom *= denom;
	if (denom < EPSILON)
		return 1.0;
	double reflection = (numer*numer) / denom;
	if (reflection > 1.0)
		return 1.0;
	return reflection;
}

int Scene::findClosestIntersection(const Vector& vantage, const Vector& direction, Intersection& intersection) const
{
	cached_intersection_list.clear();
	SolidObjectList::const_iterator iter = solid_object_list.begin();
	SolidObjectList::const_iterator end  = solid_object_list.end();
	for (; iter != end; iter++)
	{
		const SolidObject& solid = *(*iter);
		solid.appendAllIntersections(vantage, direction, cached_intersection_list);
	}
	return pickClosestIntersection(cached_intersection_list, intersection);
}

bool Scene::hasClearLineOfSight(const Vector& point1, const Vector& point2) const
{
	const Vector dir = point2 - point1;
	const double gap_distance_squared = dir.magnitudeSquared();

	SolidObjectList::const_iterator iter = solid_object_list.begin();
	SolidObjectList::const_iterator end  = solid_object_list.end();
	for (; iter != end; iter++)
	{
		const SolidObject &solid = *(*iter);
		Intersection closest;

		if (0 != solid.findClosestIntersection(point1, dir, closest))
		{
			if (closest.distance_squared < gap_distance_squared)
				return false;
		}
	}

	return true;  
}

ImageBuffer Scene::renderImage(size_t pixels_width, size_t pixels_height, double zoom, size_t anti_alias_factor) const
{
	const size_t large_pixel_width = anti_alias_factor * pixels_width;
	const size_t large_pixel_height = anti_alias_factor * pixels_height;
	const size_t small_dim = ((pixels_width < pixels_height) ? pixels_width : pixels_height);

	const double large_zoom  = anti_alias_factor * zoom * small_dim;
	ImageBuffer buffer(large_pixel_width, large_pixel_height, background_color);

	Vector camera(0.0, 0.0, 0.0);
	Vector direction(0.0, 0.0, -1.0);
	const Color full_intensity(1.0, 1.0, 1.0);
	PixelList ambiguous_pixel_list;

	for (size_t j = 0; j < large_pixel_width; j++)
	{
		direction.x = (j - large_pixel_width/2.0) / large_zoom;
		for (size_t i = 0; i < large_pixel_height; i++)
		{
			direction.y = (large_pixel_height/2.0 - i) / large_zoom;
			PixelData& pixel = buffer.pixel(i,j);
			try
			{
				pixel.color = traceRay(camera, direction, ambient_refraction, full_intensity, 0);
			}
			catch (AmbiguousIntersectionException)
			{
				pixel.is_ambiguous = true;
				ambiguous_pixel_list.push_back(PixelCoordinates(i, j));
			}
		}
	}

	PixelList::const_iterator iter = ambiguous_pixel_list.begin();
	PixelList::const_iterator end  = ambiguous_pixel_list.end();
	for (; iter != end; iter++)
	{
		const PixelCoordinates& p = *iter;
		resolveAmbiguousPixel(buffer, p.i, p.j);
	}

	const double patch_size = anti_alias_factor * anti_alias_factor;
	ImageBuffer result(pixels_width, pixels_height, background_color);

	for (size_t i = 0; i < pixels_height; i++)
	{
		for (size_t j = 0; j < pixels_width; j++)
		{
			Color sum(0.0, 0.0, 0.0);
			for (size_t di = 0; di < anti_alias_factor; di++)
			{
				for (size_t dj = 0; dj < anti_alias_factor; dj++)
					sum += buffer.pixel(anti_alias_factor * i + di, anti_alias_factor * j + dj).color;
			}
			sum /= patch_size;
			result.pixel(i, j).color = sum;
		}
	}

	return result;
}

const SolidObject* Scene::primaryContainer(const Vector &point) const
{
	SolidObjectList::const_iterator iter = solid_object_list.begin();
	SolidObjectList::const_iterator end  = solid_object_list.end();
	for (; iter != end; iter++)
	{
		const SolidObject* solid = *iter;
		if (solid->contains(point))
			return solid;
	}

	return nullptr;
}

void Scene::resolveAmbiguousPixel(ImageBuffer &buffer, size_t i, size_t j) const
{
	const size_t iMin = (i > 0) ? (i - 1) : i;
	const size_t iMax = (i < buffer.getPixelsWidth() - 1) ? (i + 1) : i;
	const size_t jMin = (j > 0) ? (j - 1) : j;
	const size_t jMax = (j < buffer.getPixelsHeight() - 1) ? (j + 1) : j;

	Color colorSum(0.0, 0.0, 0.0);
	int count = 0;
	for (size_t si = iMin; si <= iMax; si++)
	{
		for (size_t sj = jMin; sj <= jMax; sj++)
		{
			const PixelData& pixel = buffer.pixel(si, sj);
			if (!pixel.is_ambiguous)
			{
				count++;
				colorSum += pixel.color;
			}
		}
	}

	if (count > 0)
		colorSum /= count;

	buffer.pixel(i, j).color = colorSum;
}

