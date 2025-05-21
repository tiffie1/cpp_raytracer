#ifndef CONE_H
#define CONE_H

#include "Object3D.h"
#include <limits>

class Cone : public Object3D {
public:
  Cone(Vector apex, Vector axis, double height, double radius, Vector color,
       double specular, double reflective, double transparency,
       double refractiveIdx);

  ~Cone();

  std::array<double, 2> intersect(Vector &origin, Vector &direction) const;
  Vector normal(Vector &intersect_point) const;

  void shift(Vector displacement_vec);
  void rotate(double yaw, double pitch, double roll);

  Vector getCenter() const;
  Vector getColor() const;
  double getSpecular() const;
  double getReflective() const;
  double getTransparency() const;
  double getRefractiveIdx() const;

private:
  Vector apex;
  Vector axis; // normalized direction from apex to base center
  double height;
  double radius;
  Vector color;
  double specular;
  double reflective;
  double transparency;
  double refractiveIdx_;
  static constexpr double EPS = 1e-6;
  static constexpr double INF = std::numeric_limits<double>::infinity();
};

#endif // CONE_H
