#ifndef SCENE_H
#define SCENE_H

#include "Light.h"
#include "Sphere.h"
#include "Vector.h"
#include <map>
#include <string>
#include <vector>

struct ShadowCacheEntry {
  Vector point;
  Vector light_dir;
  bool hit_shadow;
};

class Scene {
private:
  std::string scene_identification;
  Vector bg_color;

public:
  std::vector<Light> lights;
  std::vector<Sphere *> objects;
  ShadowCacheEntry * recently_shaded;

  Scene(Vector camera_origin, std::string scene_identify,
        Vector background_color);
  ~Scene();

  Vector getBackgroundColor() const;
};

#endif // !SCENE_H
