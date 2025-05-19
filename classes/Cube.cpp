#include "Cube.h"
#include "Inf.h" // for INF

Cube::Cube(const Vector& center, double side_length,
           const Vector& color, double specular,
           double reflective, double transparency,
           double refractiveIdx)
    : center_(center),
      half_size_(side_length * 0.5),
      color_(color),
      specular_(specular),
      reflective_(reflective),
      transparency_(transparency),
      refractiveIdx_(refractiveIdx) {}

std::array<double, 2> Cube::intersect(Vector &origin,
                                      Vector &direction) const {
    // Compute intersection of ray (origin, direction) with an axis-aligned cube
    Vector bounds_min = center_ - Vector(half_size_, half_size_, half_size_);
    Vector bounds_max = center_ + Vector(half_size_, half_size_, half_size_);

    double tmin = -INF;
    double tmax = INF;

    // Slab method for x, y, z
    for (int i = 0; i < 3; ++i) {
        double o = origin[i];
        double d = direction[i];
        double bmin = bounds_min[i];
        double bmax = bounds_max[i];

        if (std::abs(d) < 1e-8) {
            // Ray parallel to slab; no hit if origin not within bounds
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
    // Determine which face is hit by comparing distances to center
    Vector local = p - center_;
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
    center_ = center_ + displacement_vec;
}

void Cube::rotate(double yaw, double pitch, double roll) {
    // Rotation for axis-aligned cube not supported.
    // For an oriented box, you'd apply an inverse rotation to incoming rays.
}

Vector Cube::getCenter() const { return center_; }
Vector Cube::getColor() const { return color_; }
double Cube::getSpecular() const { return specular_; }
double Cube::getReflective() const { return reflective_; }
double Cube::getTransparency() const { return transparency_; }
double Cube::getRefractiveIdx() const { return refractiveIdx_; }
