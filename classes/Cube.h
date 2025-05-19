#include "Object3D.h"
#include "Vector.h"

class Cube : public Object3D {
private:
    Vector  center_;
    double  half_size_;  // half of side length
    Vector  color_;
    double  specular_;
    double  reflective_;
    double  transparency_;
    double  refractiveIdx_;

public:
    // center: cube center; side_length: full edge length
    Cube(const Vector& center, double side_length,
         const Vector& color, double specular,
         double reflective, double transparency,
         double refractiveIdx);

    // Intersection with axis-aligned cube (AABB) using slab method
    std::array<double, 2> intersect(Vector &origin,
                                    Vector &direction) const override;

    // Surface normal at a given intersection point
    Vector normal(Vector &intersect_point) const override;

    // Translate and rotate (rotate is a no-op for AABB)
    void shift(Vector displacement_vec) override;
    void rotate(double yaw, double pitch, double roll) override;

    // Getters
    Vector getCenter() const override;
    Vector getColor() const override;
    double getSpecular() const override;
    double getReflective() const override;
    double getTransparency() const override;
    double getRefractiveIdx() const override;
};
