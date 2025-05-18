#ifndef SPHERE_H
#define SPHERE_H

#include "Inf.h"
#include "Vector.h"
#include <array>

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
  double refractive_idx;
  double transparency;

public:
  Sphere();
  Sphere(Vector camera_origin, Vector center_val, double radius_val,
         Vector color_vec, float specular_val, float reflect_val,
         double refracive_idx, double transparency);
  ~Sphere();

  Vector getCenter() const;
  Vector getOffset() const;
  double getRadius() const;
  double getSquaredRadius() const;
  Vector getColor() const;
  double getSpecular() const;
  double getReflective() const;
  bool is_defined() const;
  double getRefractiveIdx() const ;
  double getTransparency() const ;

  Sphere &operator=(const Sphere &other);

  inline std::array<double, 2> intersect(Vector &origin, Vector &direction,
                                         double direction_dot) const {
    double a = direction_dot;
    double b, c;

    if (origin == Vector(0, 0, 0)) {
      b = offset.dot(direction);
      c = offset_dot - square_radius;
    } else {
      Vector OC = origin - center;
      b = OC.dot(direction);
      c = OC.dot(OC) - square_radius;
    }

    double discriminant = b * b - a * c;

    if (discriminant < 0)
      return {INF, INF};
    else {
      double root = sqrt(discriminant);
      double t1 = (-b + root) / a;
      double t2 = (-b - root) / a;

      return {t1, t2};
    }
  }

  Vector normal(Vector intersect_point) const;
  void shift(Vector displacement_vec);

  friend std::ostream &operator<<(std::ostream &stream, const Sphere &sphere);
};

#endif // !SPHERE_H
