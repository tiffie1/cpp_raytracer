#include "Sphere.h"

Sphere::Sphere() {
  center = Vector(0, 0, 0);
  radius = 0;
  square_radius = 0;
  color = Vector(0, 0, 0);
  specular = 0;
  reflective = 0;
  is_valid = false;
  // sss_albedo = Vector();
  // sss_distance = 0;
}

Sphere::~Sphere() {}

Sphere::Sphere(Vector camera_origin, Vector center_val, double radius_val,
               Vector color_vec, float specular_val, float reflect_val,
               double refractive_idx_val, double transparency_val) {
  // double sss_distance_val, Vector sss_albedo_val) {
  center = center_val;
  offset = camera_origin - center;
  offset_dot = offset.dot(offset);
  radius = radius_val;
  square_radius = radius * radius;
  color = color_vec;
  specular = specular_val;
  reflective = reflect_val;
  is_valid = true;
  refractive_idx = refractive_idx_val;
  transparency = transparency_val;
  // sss_distance = sss_distance_val;
  // sss_albedo = sss_albedo_val;
}

Vector Sphere::getCenter() const { return center; }
Vector Sphere::getOffset() const { return offset; }
double Sphere::getRadius() const { return radius; }
Vector Sphere::getColor() const { return color; }
double Sphere::getSpecular() const { return specular; }
double Sphere::getReflective() const { return reflective; }
bool Sphere::is_defined() const { return is_valid; }
double Sphere::getRefractiveIdx() const { return refractive_idx; }
double Sphere::getTransparency() const { return transparency; }
// double Sphere::getSSSDistance() const { return sss_distance; }
// Vector Sphere::getSSSAlbedo() const { return sss_albedo; }

Vector Sphere::normal(Vector &intersect_point) const {
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
