#include "Light.h"

Light::Light() {
  type = "";
  intensity = 0;
  behaviour = Vector();
}

Light::Light(std::string type_str, double intensity_val) {
  type = type_str;
  intensity = intensity_val;
}

Light::Light(std::string type_str, double intensity_val, Vector behaviour_vec) {
  type = type_str;
  intensity = intensity_val;
  behaviour = behaviour_vec;
}

std::string Light::getType() const { return type; }
double Light::getIntensity() const { return intensity; }
Vector Light::getBehaviour() const { return behaviour; }
