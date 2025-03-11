#include <Vector.h>

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

double Vector::dot(const Vector &other) const {
  return x * other.x + y * other.y + z * other.z;
}

double Vector::norm() { return sqrt(x * x + y * y + z * z); }

std::ostream &operator<<(std::ostream &stream,
                                        const Vector &vector) {
  stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
  return stream;
}
