#include "Inf.h"
#include "Sphere.h"

Sphere::Sphere() {
  center = Vector(0, 0, 0);
  radius = 0;
  color = Vector(0, 0, 0);
  specular = 0;
  reflective = 0;
  is_valid = false;
}

Sphere::~Sphere() {}

Sphere::Sphere(Vector center_val, double radius_val, Vector color_vec,
               float specular_val, float reflect_val) {
  center = center_val;
  radius = radius_val;
  color = color_vec;
  specular = specular_val;
  reflective = reflect_val;
  is_valid = true;
}

Vector Sphere::getCenter() const { return center; }
double Sphere::getRadius() const { return radius; }
Vector Sphere::getColor() const { return color; }
double Sphere::getSpecular() const { return specular; }
double Sphere::getReflective() const { return reflective; }
bool Sphere::is_defined() const { return is_valid; }

std::array<double, 2> Sphere::intersect(Vector origin, Vector direction) const {
  Vector coord_offset = origin - center;

  double a = direction.dot(direction);
  double b = 2 * (coord_offset.dot(direction));
  double c = coord_offset.dot(coord_offset) - (radius * radius);

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    const std::array<double, 2> result = {INF, INF};
    return result;
  }

  double t1 = (-b + sqrt(discriminant)) / (2 * a);
  double t2 = (-b - sqrt(discriminant)) / (2 * a);

  const std::array<double, 2> result = {t1, t2};
  return result;
}

Vector Sphere::normal(Vector intersect_point) const {
  return (intersect_point - center) / radius;
}

void Sphere::shift(Vector displacement_vec) {
  center = center + displacement_vec;
}

Sphere &Sphere::operator=(const Sphere &other) {
  if (this != &other) { // Self-assignment check
    center = other.center;
    radius = other.radius;
    color = other.color;
    specular = other.specular;
    reflective = other.reflective;
    is_valid = true;
  }
  return *this;
}

std::ostream &operator<<(std::ostream &stream, const Sphere &sphere) {
  stream << "Center: " << sphere.center << "\nRadius: " << sphere.radius
         << "\nColor: " << sphere.color;

  return stream;
}
