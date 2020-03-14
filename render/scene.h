#ifndef SCENE_H
#define SCENE_H

#include "image.h"
#include "intersection.h"
#include "light.h"
#include "solid_object.h"

typedef std::vector<SolidObject*> SolidObjectList;
typedef std::vector<Light> LightList;

struct PixelCoordinates
{
	size_t i;
	size_t j;

	PixelCoordinates(size_t _i, size_t _j): i(_i), j(_j) {}
};

class AmbiguousIntersectionException
{
};

typedef std::vector<PixelCoordinates> PixelList;

class Scene
{
public:
	explicit Scene(const Color& _background_color = Color());
	virtual ~Scene();

	SolidObject &addSolidObject(SolidObject *solid_object);

	void addLight(const Light &light);
	ImageBuffer renderImage(size_t pixels_width, size_t pixels_height, double zoom, size_t anti_alias_factor) const;
	void setAmbientRefraction(double refraction);

private:
	void clearSolidObjectList();
	int findClosestIntersection(const Vector &vantage, const Vector &direction, Intersection &intersection) const;
	bool hasClearLineOfSight(const Vector &point1, const Vector &point2) const;
	Color traceRay(const Vector &vantage, const Vector &direction, double refractive_index, Color ray_intensity, int recursion_depth) const;
	Color calculateLighting(const Intersection &intersection, const Vector &direction, double refractive_index, Color ray_intensity, int recursion_depth) const;
	Color calculateMatte(const Intersection &intersection) const;
	Color calculateReflection(const Intersection &intersection, const Vector &incidentDir, double refractive_index, Color ray_intensity, int recursion_depth) const;
	Color calculateRefraction(const Intersection &intersection, const Vector &direction, double source_refractive_index, Color ray_intensity, int recursion_depth, double &out_reflection_factor) const;
	const SolidObject *primaryContainer(const Vector &point) const;
	double polarizedReflection(double n1, double n2, double cos_a1, double cos_a2) const;
	void resolveAmbiguousPixel(ImageBuffer &buffer, size_t i, size_t j) const;

	Color background_color;                  

	SolidObjectList solid_object_list;
	LightList light_list;
	double ambient_refraction;

	mutable IntersectionList cached_intersection_list;
};

#endif
