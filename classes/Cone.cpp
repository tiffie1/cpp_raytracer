#include "Cone.h"
#include <cmath>

Cone::Cone( Vector apex, Vector axis, double height, double radius,
           Vector color, double specular, double reflective,
           double transparency, double refractiveIdx)
    : apex(apex), axis(axis.normalized()), height(height), radius(radius),
      color(color), specular(specular), reflective(reflective),
      transparency(transparency), refractiveIdx_(refractiveIdx) {}

Cone::~Cone() {}

std::array<double, 2> Cone::intersect(Vector &origin, Vector &direction) const {
  double cosTheta = height / std::sqrt(height * height + radius * radius);
  double k = cosTheta * cosTheta;

  Vector co = origin - apex;
  double dv = direction.dot(axis);
  double cov = co.dot(axis);
  double d_dot_d = direction.dot(direction);
  double co_dot_d = co.dot(direction);
  double co_dot_co = co.dot(co);

  double a = dv * dv - k * d_dot_d;
  double b = 2 * (dv * cov - k * co_dot_d);
  double c = cov * cov - k * co_dot_co;

  double disc = b * b - 4 * a * c;
  if (disc < 0)
    return {INF, INF};

  double sqrt_disc = std::sqrt(disc);
  double t1 = (-b + sqrt_disc) / (2 * a);
  double t2 = (-b - sqrt_disc) / (2 * a);

  auto valid = [&](double t) {
    if (t < EPS)
      return false;
    Vector p = origin + direction * t;
    double dist = (p - apex).dot(axis);
    return dist >= 0.0 && dist <= height;
  };

  double rt1 = valid(t1) ? t1 : INF;
  double rt2 = valid(t2) ? t2 : INF;
  if (rt1 < rt2)
    return {rt1, rt2};
  return {rt2, rt1};
}

Vector Cone::normal(Vector &intersect_point) const {
  Vector v = intersect_point - apex;
  double h = v.dot(axis);
  Vector radial = v - axis * h;
  double radialLen = radial.norm();

  Vector n = (radial * (height / radius)) + (axis * radialLen);
  return n.normalized();
}

void Cone::shift(Vector displacement_vec) { apex = apex + displacement_vec; }

void Cone::rotate(double yaw, double pitch, double roll) {
  apex = apex.rotate(yaw, pitch, roll);
  axis = axis.rotate(yaw, pitch, roll).normalized();
}

Vector Cone::getCenter() const { return apex + axis * height; }

Vector Cone::getColor() const { return color; }
double Cone::getSpecular() const { return specular; }
double Cone::getReflective() const { return reflective; }
double Cone::getTransparency() const { return transparency; }
double Cone::getRefractiveIdx() const { return refractiveIdx_; }
