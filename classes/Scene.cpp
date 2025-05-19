#include "Scene.h"
#include "Cube.h"
#include "Light.h"
#include "Sphere.h"
#include "Cone.h"

Scene::Scene(Vector camera_origin, std::string scene_identify,
             Vector background_color)
    : scene_identification(scene_identify) {
  bg_color = background_color;
  recently_shaded =
      new ShadowCacheEntry({Vector(0, 0, 0), Vector(0, 0, 0), false});

  if (scene_identification == "basic") {

    objects.push_back(new Sphere(camera_origin, Vector(0, -1, 4), 1,
                                 Vector(255, 0, 0), 500, 0.2, 0, 0.0));

    objects.push_back(new Sphere(camera_origin, Vector(2, 0, 4), 1,
                                 Vector(0, 255, 0), 500, 0.3, 0, 0.0));

    objects.push_back(new Sphere(camera_origin, Vector(-2, 0, 4), 1,
                                 Vector(0, 0, 255), 500, 0.5, 0, 0.0));

    objects.push_back(new Sphere(camera_origin, Vector(0, -5001, 0), 5000,
                                 Vector(255, 255, 0), 1000, 0.1, 0, 0.0));

    lights.push_back(Light("ambient", 0.2));
    lights.push_back(Light("point", 0.6, Vector(2, 1, 0)));
    lights.push_back(Light("directional", 0.2, Vector(1, 4, 4)));
  } else if (scene_identification == "orbit") {
    // objects.push_back(
    // new Cube(Vector(0, 1.5, 5), 1, Vector(0, 255, 0), 0.5, 500, 0.0, 0));

    objects.push_back(new Sphere(camera_origin, Vector(0, 0, 5), 1,
                                 Vector(255, 0, 0), 500, 0.2, 0, 0.0));

    objects.push_back(
        new Cube(Vector(0, -1.5, 5), 2.0, // center & side length = 2
                 Vector(0.5, 0.5, 0.5),   // gray color
                 100.0, 0.3, 0.0,
                 0.0) // specular, reflective, transparency, ior
    );

    objects.push_back(new Cube(Vector(3, 0, 5), 1.0, Vector(200, 200, 200),
                               50.0, 0.1, 0.0, 0.0));
    objects.push_back(new Cube(Vector(-3, 0, 5), 1.0, Vector(200, 200, 200),
                               50.0, 0.1, 0.0, 0.0));
    objects.push_back(new Cube(Vector(0, 3, 5), 1.0, Vector(200, 200, 200),
                               50.0, 0.1, 0.0, 0.0));

    objects.push_back(new Sphere(camera_origin, Vector(0, -5001, 0), 5000,
                                 Vector(255, 255, 0), 1000, 0.1, 0, 0.0));

    lights.push_back(Light("ambient", 0.2));
    lights.push_back(Light("point", 0.6, Vector(2, 1, 0)));
    lights.push_back(Light("directional", 0.2, Vector(1, 4, 4)));
  } else if (scene_identification == "chat") {

    objects.push_back(new Cone(Vector(0, 0, 5), Vector(0, -1, 0), 2, 1,
                               Vector(255, 0, 0), 100, 0.1, 0.0, 0));

    objects.push_back(new Sphere(camera_origin, Vector(0, -5001, 0), 5000,
                                 Vector(255, 255, 255), 1000, 0.1, 0, 0.0));

    lights.push_back(Light("ambient", 0.2));
    lights.push_back(Light("point", 0.6, Vector(2, 1, 0)));
    lights.push_back(Light("directional", 0.2, Vector(1, 4, 4)));
  }
}

Scene::~Scene() {
  delete recently_shaded;

  for (Object3D *sphere : objects)
    delete sphere;

  objects.clear();
  lights.clear();
}

Vector Scene::getBackgroundColor() const { return bg_color; }
