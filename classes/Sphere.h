#ifndef SPHERE_H
#define SPHERE_H

#include "Vector.h"
#include <fstream>

class Sphere {
private:
  Vector center;
  Vector offset;
  double offset_dot;
  double radius;
  double square_radius;
  Vector color;
  double specular;
  double reflective;
  bool is_valid;

public:
  Sphere();
  Sphere(Vector camera_origin, Vector center_val, double radius_val, Vector color_vec,
         float specular_val, float reflect_val);
  ~Sphere();

  Vector getCenter() const;
  Vector getOffset() const;
  double getRadius() const;
  double getSquaredRadius() const;
  Vector getColor() const;
  double getSpecular() const;
  double getReflective() const;
  bool is_defined() const;

  Sphere &operator=(const Sphere &other);

  std::array<double, 2> intersect(Vector origin, Vector direction, double direction_dot) const;

  Vector normal(Vector intersect_point) const;
  void shift(Vector displacement_vec);

  friend std::ostream &operator<<(std::ostream &stream, const Sphere &sphere);
};

#endif // !SPHERE_H
