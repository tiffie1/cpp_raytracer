#include "Vector.h"

Vector::Vector() {
  x = 0;
  y = 0;
  z = 0;
}

Vector::Vector(double x_val, double y_val, double z_val) {
  x = x_val;
  y = y_val;
  z = z_val;
}

Vector Vector::operator+(const Vector &other) {
  return Vector(x + other.x, y + other.y, z + other.z);
}

Vector Vector::operator-(const Vector &other) {
  return Vector(x - other.x, y - other.y, z - other.z);
}

Vector Vector::operator/(const Vector &other) {
  return Vector(x / other.x, y / other.y, z / other.z);
}

Vector Vector::operator*(const Vector &other) {
  return Vector(x * other.x, y * other.y, z * other.z);
}

Vector Vector::operator/(const double &scalar) {
  return Vector(x / scalar, y / scalar, z / scalar);
}

Vector Vector::operator*(const double &scalar) {
  return Vector(x * scalar, y * scalar, z * scalar);
}

Vector Vector::operator-() const { return Vector(-x, -y, -z); }

Vector &Vector::operator=(const Vector &other) {
  if (this != &other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  return *this;
}

bool Vector::operator==(const Vector &other) {
  if (x == other.x && y == other.y && z == other.z)
    return true;
  else
    return false;
}

  double Vector::dot(const Vector &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  double Vector::norm() { return sqrt(x * x + y * y + z * z); }

  std::ostream &operator<<(std::ostream &stream, const Vector &vector) {
    stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return stream;
  }

    double& Vector::operator[](std::size_t i) {
        switch (i) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default:
                throw std::out_of_range("Vector index out of range");
        }
    }

Vector Vector::normalized() const {
    double len = sqrt(x * x + y * y + z * z);
    if (len == 0.0) {
        // zero‐vector stays zero (or you could choose to assert)
        return Vector(0.0, 0.0, 0.0);
    }
    return Vector(x/len, y/len, z/len);
}

Vector Vector::cross(const Vector &other) const {
    return Vector(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x
    );
}

