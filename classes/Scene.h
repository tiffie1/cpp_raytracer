#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Object3D.h"
#include <string>

class Scene {
private:
  std::string scene_identification;

public:
  std::array<Light, 3> lights;
  std::array<Object3D *, 7> objects;

  Scene(std::string scene_identify);
};

#endif // !SCENE_H
