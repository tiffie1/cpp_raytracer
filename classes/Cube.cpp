#include "Cube.h"
#include "Inf.h" // for INF

Cube::Cube(const Vector& center, double side_length,
           const Vector& color, double specular,
           double reflective, double transparency,
           double refractiveIdx)
    : center(center),
      half_size(side_length * 0.5),
      color(color),
      specular(specular),
      reflective(reflective),
      transparency(transparency),
      refractiveIdx(refractiveIdx) {}

std::array<double, 2> Cube::intersect(Vector &origin,
                                      Vector &direction) const {
    // Compute intersection of ray (origin, direction) with an axis-aligned cube
    Vector bounds_min = center - Vector(half_size, half_size, half_size);
    Vector bounds_max = center + Vector(half_size, half_size, half_size);

    double tmin = -INF;
    double tmax = INF;

    for (int i = 0; i < 3; ++i) {
        double o = origin[i];
        double d = direction[i];
        double bmin = bounds_min[i];
        double bmax = bounds_max[i];

        if (std::abs(d) < 1e-8) {
            if (o < bmin || o > bmax)
                return {INF, INF};
        } else {
            double t1 = (bmin - o) / d;
            double t2 = (bmax - o) / d;
            if (t1 > t2) std::swap(t1, t2);
            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);
            if (tmin > tmax)
                return {INF, INF};
        }
    }
    return {tmin, tmax};
}

Vector Cube::normal(Vector &p) const {
    Vector local = p - center;
    double absx = std::abs(local.x);
    double absy = std::abs(local.y);
    double absz = std::abs(local.z);

    if (absx > absy && absx > absz) {
        return Vector((local.x > 0.0) ? 1.0 : -1.0, 0.0, 0.0);
    } else if (absy > absx && absy > absz) {
        return Vector(0.0, (local.y > 0.0) ? 1.0 : -1.0, 0.0);
    } else {
        return Vector(0.0, 0.0, (local.z > 0.0) ? 1.0 : -1.0);
    }
}

void Cube::shift(Vector displacement_vec) {
    center = center + displacement_vec;
}

void Cube::rotate(double yaw, double pitch, double roll) {
    // Rotation for axis-aligned cube not supported.
}

Vector Cube::getCenter() const { return center; }
Vector Cube::getColor() const { return color; }
double Cube::getSpecular() const { return specular; }
double Cube::getReflective() const { return reflective; }
double Cube::getTransparency() const { return transparency; }
double Cube::getRefractiveIdx() const { return refractiveIdx; }
