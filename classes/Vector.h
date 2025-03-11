#ifndef VECTOR_H

#define VECTOR_H
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

  double dot(const Vector &other) const;
  double norm();
  friend std::ostream &operator<<(std::ostream &stream, const Vector &vector);
};

#endif // VECTOR_H
