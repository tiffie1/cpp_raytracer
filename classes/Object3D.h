#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Vector.h"
#include <array>

class Object3D {
public:
  virtual ~Object3D() {}

  virtual Vector normal(Vector &intersect_point) const = 0;
  virtual std::array<double, 2> intersect(Vector &origin,
                                          Vector &direction) const = 0;

  virtual void shift(Vector displacement_vec) = 0;
  virtual void rotate(double yaw, double pitch, double roll) = 0;

  virtual Vector getCenter() const = 0;
  virtual Vector getColor() const = 0;
  virtual double getSpecular() const = 0;
  virtual double getReflective() const = 0;
  virtual double getTransparency() const = 0;
  virtual double getRefractiveIdx() const = 0;
  //virtual double getSSSDistance() const = 0;
  //virtual Vector getSSSAlbedo() const = 0;
};

#endif
