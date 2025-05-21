#include "Object3D.h"
#include "Vector.h"

class Cube : public Object3D {
private:
    Vector  center;
    double  half_size;  // half of side length
    Vector  color;
    double  specular;
    double  reflective;
    double  transparency;
    double  refractiveIdx;

public:
    Cube(const Vector& center, double side_length,
         const Vector& color, double specular,
         double reflective, double transparency,
         double refractiveIdx);

    std::array<double, 2> intersect(Vector &origin,
                                    Vector &direction) const override;

    Vector normal(Vector &intersect_point) const override;

    void shift(Vector displacement_vec) override;
    void rotate(double yaw, double pitch, double roll) override;

    Vector getCenter() const override;
    Vector getColor() const override;
    double getSpecular() const override;
    double getReflective() const override;
    double getTransparency() const override;
    double getRefractiveIdx() const override;
};
