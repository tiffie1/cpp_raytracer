#include "Scene.h"
#include "Light.h"
#include "Sphere.h"

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
  }
}

Scene::~Scene() {
  delete recently_shaded; 

  for (Object3D * sphere : objects)
    delete sphere;

  objects.clear();
  lights.clear();
}

Vector Scene::getBackgroundColor() const { return bg_color; }
