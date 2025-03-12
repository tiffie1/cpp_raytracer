#include "Plane.h"
#include "Inf.h"
#include "Vector.h"

Plane::Plane()
    : coefficients(Vector()), color(Vector()), position_constant(0),
      specular(0) {}

Plane::~Plane() {}

Plane::Plane(const char plane_type[2], Vector plane_color,
             double positional_const, double spec_value, double reflect_val) {
  unsigned int coeff_arr[3] = {0, 0, 0};

  // Only keep component that isn't in the plane_type.
  coeff_arr[0] = (plane_type[0] != 'x' && plane_type[1] != 'x');
  coeff_arr[1] = (plane_type[0] != 'y' && plane_type[1] != 'y');
  coeff_arr[2] = (plane_type[0] != 'z' && plane_type[1] != 'z');

  coefficients = Vector(coeff_arr[0], coeff_arr[1], coeff_arr[2]);
  color = plane_color;
  position_constant = positional_const;
  specular = spec_value;
  reflective = reflect_val;
}

Vector Plane::getCoefficients() const { return coefficients; }
Vector Plane::getColor() const { return color; }
double Plane::getPositionConstant() const { return position_constant; }
double Plane::getSpecular() const { return specular; }
double Plane::getReflective() const { return reflective; }
Vector Plane::getCenter() const {
  return Vector(position_constant, position_constant, position_constant) *
         coefficients;
}

void Plane::shift(Vector displacement_vec) {
  double value;
  if (coefficients.x != 0)
    value = displacement_vec.x;
  else if (coefficients.y != 0)
    value = displacement_vec.y;
  else
    value = displacement_vec.z;

  position_constant += value;
}

std::array<double, 2> Plane::intersect(Vector origin, Vector direction) const {
  double D = -position_constant;

  double nominator = -(coefficients.dot(origin) + D);
  double denominator = coefficients.dot(direction);

  if (denominator == 0) {
    const std::array<double, 2> result = {INF, INF};
    return result;
  }

  double t1 = nominator / denominator;

  const std::array<double, 2> result = {t1, INF};
  return result;
}

Vector Plane::normal(Vector intersect_point) const {
  intersect_point.norm(); // supress unused parameter warning

  if (position_constant >= 0)

    return -coefficients;
  else
    return coefficients;
}

Plane &Plane::operator=(const Plane &other) {

  coefficients = other.coefficients;
  color = other.color;
  position_constant = other.position_constant;
  specular = other.specular;

  return *this;
}
