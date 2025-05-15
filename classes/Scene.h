#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Sphere.h"
#include "Vector.h"
#include <map>
#include <string>

class Scene {
private:
  std::string scene_identification;
  Vector bg_color;
  std::map<double, double> square_radii;

public:
  std::array<Light, 3> lights;
  std::array<Sphere *, 7> objects;

  Scene(std::string scene_identify, Vector background_color);
  ~Scene();

  Vector getBackgroundColor() const;
};

#endif // !SCENE_H
