#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <fstream>

class Vector {
public:
  double x, y, z;

  Vector();
  Vector(double x_val, double y_val, double z_val);

  Vector operator+(const Vector &other);
  Vector operator-(const Vector &other);
  Vector operator-() const;
  Vector operator/(const Vector &other);
  Vector operator/(const double &scalar);
  Vector operator*(const Vector &other);
  Vector operator*(const double &scalar);
  Vector &operator=(const Vector &other);
  bool operator==(const Vector &other);
  double &operator[](std::size_t i);

  Vector operator*(double scalar) const;

  Vector operator-(const Vector &rhs) const {
    return Vector{x - rhs.x, y - rhs.y, z - rhs.z};
  }

  Vector operator+(const Vector &rhs) const {
    return Vector{x + rhs.x, y + rhs.y, z + rhs.z};
  }

  double dot(const Vector &other) const;
  double norm();
  Vector normalized() const;
  Vector cross(const Vector &other) const;
  Vector rotate(double yawDeg, double pitchDeg, double rollDeg) const;
  friend std::ostream &operator<<(std::ostream &stream, const Vector &vector);
};

#endif // !VECTOR_H
