#include "Scene.h"
#include "Light.h"
#include "Sphere.h"
#include <array>
#include <set>

std::map<double, double> calculate_squares(std::array<Sphere *, 7> &array) {
  std::set<double> radii;

  for (unsigned short i = 0; i < 7; i++) {
    if (Sphere *a = dynamic_cast<Sphere *>(array[i])) {
      radii.insert(a->getRadius());
    }
  }

  std::map<double, double> result;
  for (double r : radii) {
    result[r] = r * r;
  }

  return result;
}

Scene::Scene(std::string scene_identify, Vector background_color)
    : scene_identification(scene_identify) {
  bg_color = background_color;

  if (scene_identification == "basic") {
    std::array<Sphere *, 7> objects_arr = {
        {new Sphere(Vector(0, -1, 3), 1, Vector(255, 0, 0), 500, 0.2),
         new Sphere(Vector(2, 0, 4), 1, Vector(0, 255, 0), 500, 0.3),
         new Sphere(Vector(-2, 0, 4), 1, Vector(0, 0, 255), 500, 0.5),
         new Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 0), 1000, 0.1),
         new Sphere(), new Sphere(), new Sphere()}};
    objects = objects_arr;

    std::array<Light, 3> lights_arr = {
        {Light("ambient", 0.2), Light("point", 0.6, Vector(2, 1, 0)),
         Light("directional", 0.2, Vector(1, 4, 4))}};

    lights = lights_arr;
  } else {
    std::array<Sphere *, 7> objects_arr = {{}};
    std::array<Light, 3> lights_arr = {{}};

    objects = objects_arr;
    lights = lights_arr;
  }

  square_radii = calculate_squares(objects);
}

Scene::~Scene() {}

Vector Scene::getBackgroundColor() const { return bg_color; }
