#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Sphere.h"
#include "Vector.h"
#include <vector>
#include <map>
#include <string>

class Scene {
private:
  std::string scene_identification;
  Vector bg_color;
  std::map<double, double> square_radii;

public:
  std::vector<Light> lights;
  std::vector<Sphere *> objects;

  Scene(std::string scene_identify, Vector background_color);
  ~Scene();

  Vector getBackgroundColor() const;
};

#endif // !SCENE_H
