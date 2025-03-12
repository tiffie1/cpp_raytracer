#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Object3D.h"
#include "Vector.h"
#include <string>

class Scene {
private:
  std::string scene_identification;
  Vector bg_color;

public:
  std::array<Light, 3> lights;
  std::array<Object3D *, 7> objects;

  Scene(std::string scene_identify, Vector background_color);
  ~Scene();

  Vector getBackgroundColor() const;
};

#endif // !SCENE_H
