#ifndef SPHERE_H
#define SPHERE_H

#include "Inf.h"
#include "Object3D.h"
#include "Vector.h"
#include <array>

class Sphere : public Object3D {
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
  // double sss_distance;
  // Vector sss_albedo;

public:
  Sphere();
  Sphere(Vector camera_origin, Vector center_val, double radius_val,
         Vector color_vec, float specular_val, float reflect_val,
         double refracive_idx, double transparency);
  // double sss_distance_val, Vector sss_albedo_val);
  ~Sphere();

  Vector getCenter() const;
  Vector getOffset() const;
  double getRadius() const;
  double getSquaredRadius() const;
  Vector getColor() const;
  double getSpecular() const;
  double getReflective() const;
  bool is_defined() const;
  double getRefractiveIdx() const;
  double getTransparency() const;
  // double getSSSDistance() const;
  // Vector getSSSAlbedo() const;

  Sphere &operator=(const Sphere &other);

  Vector normal(Vector &intersect_point) const;
  void shift(Vector displacement_vec);
  void rotate(double yaw, double pitch, double roll) {}

  friend std::ostream &operator<<(std::ostream &stream, const Sphere &sphere);

  inline std::array<double, 2> intersect(Vector &origin,
                                         Vector &direction) const {
    double a = direction.dot(direction);
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
};

#endif // !SPHERE_H
