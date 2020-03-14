#include <iostream>
#include "scene.h"
#include "sphere.h"
#include "light.h"
#include "image.h"
#include "lodepng.h"
#include "triangle_object.h"
#include "constants.h"

#define IMAGE_WIDTH 1920
#define IMAGE_HEIGHT 1080

#define FILENAME "result.png"

int main(void)
{
	Scene scene(Color(0.0, 0.0, 0.0));

	Sphere *sphere1 = new Sphere(Vector(-20, 0, -100), 25.5);
	Sphere *sphere2 = new Sphere(Vector(30, 30, -120), 15.5);
	Sphere *sphere3 = new Sphere(Vector(45, 30, -120), 15.5);
	sphere1->setOptics(Optics(Color(1.0, 0.5, 0.0)));
	sphere2->setOptics(Optics(Color(0.0, 1.0, 0.5), Color(1.0, 0.5, 1.0)));
	sphere3->setOptics(Optics(Color(0.0, 1.0, 1.0), Color(1.0, 0.5, 1.0)));
	sphere3->setRefraction(REFRACTION_VACUUM);

	scene.addSolidObject(sphere1);
	scene.addSolidObject(sphere2);
	scene.addSolidObject(sphere3);

	scene.addLight(Light(Vector(50, 20, -10), Color(0.5, 0.1, 0.1, 0.15)));
	scene.addLight(Light(Vector(20, 50, -20), Color(0.2, 0.5, 0.4, 1)));

	ImageBuffer image_buffer = scene.renderImage(IMAGE_WIDTH, IMAGE_HEIGHT, 1, 3);
	std::vector<unsigned char> *result = image_buffer.convertToRaw();
	lodepng::encode(FILENAME, *result, IMAGE_WIDTH, IMAGE_HEIGHT);
	delete result;
	std::cout << "Wrote " << FILENAME << std::endl;
}
