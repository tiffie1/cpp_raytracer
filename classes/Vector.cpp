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

static constexpr double DEG2RAD = M_PI/180.0;

Vector Vector::rotate(double yawDeg, double pitchDeg, double rollDeg) const {
    // Convert to radians
    double yaw   = yawDeg   * DEG2RAD;
    double pitch = pitchDeg * DEG2RAD;
    double roll  = rollDeg  * DEG2RAD;

    // Precompute sines and cosines
    double cy = std::cos(yaw),   sy = std::sin(yaw);
    double cp = std::cos(pitch), sp = std::sin(pitch);
    double cr = std::cos(roll),  sr = std::sin(roll);

    // Build the combined rotation matrix R = Rz(roll) * Rx(pitch) * Ry(yaw)
    // Note: order matters. You can adjust based on the convention you want.
    double m00 =  cy*cr + sy*sp*sr;
    double m01 =  cr*sy*sp - cy*sr;
    double m02 =  cp*sy;

    double m10 =  cp*sr;
    double m11 =  cp*cr;
    double m12 = -sp;

    double m20 =  cy*sp*sr - cr*sy;
    double m21 =  sy*sr + cy*cr*sp;
    double m22 =  cy*cp;

    // Apply R to this vector
    return Vector(
        m00 * x + m01 * y + m02 * z,
        m10 * x + m11 * y + m12 * z,
        m20 * x + m21 * y + m22 * z
    );
}

Vector Vector::operator*(double scalar) const {
    return Vector(x * scalar,
                  y * scalar,
                  z * scalar);
}
