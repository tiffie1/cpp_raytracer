#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include "Vector.h"
#include <string>

class Plane : public Object3D {
private:
  Vector coefficients;
  Vector color;
  double position_constant;
  double specular;
  double reflective;

public:
  Plane();
  ~Plane();

  Plane(const char plane_type[2], Vector plane_color, double positional_const,
        double spec_value, double reflect_val);

  Vector getCoefficients() const;
  Vector getColor() const;
  double getPositionConstant() const;
  double getSpecular() const;
  double getReflective() const;
  Vector getCenter() const;

  Plane &operator=(const Plane &other);

  void shift(Vector displacement_vec);

  std::array<double, 2> intersect(Vector origin, Vector direction) const;

  Vector normal(Vector intersect_point) const;
};

#endif // !PLANE_H
