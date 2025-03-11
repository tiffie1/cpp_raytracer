#ifndef LIGHT_H
#define LIGHT_H

#include "Vector.h"
#include <string>

class Light {
private:
  std::string type;
  double intensity;
  Vector behaviour;

public:
  Light();
  Light(std::string type_str, double intensity_val);
  Light(std::string type_str, double intensity_val, Vector behaviour_vec);

  std::string getType() const;
  double getIntensity() const;
  Vector getBehaviour() const;
};

#endif // !LIGHT_H
