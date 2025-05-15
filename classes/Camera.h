#ifndef CAMERA_H
#define CAMERA_H

#include "Canvas.h"
#include "Scene.h"
#include "Vector.h"

#include <array>
#include <string>

class Camera {
private:
  Vector origin;
  std::array<Vector, 3> rotational_matrix;

public:
  Camera();
  Camera(Vector origin_vec);

  Vector getOrigin();

  void rotate(double yaw, double roll, double pitch);
  void move(double displacement_x, double displacement_y,
            double displacement_z);

  void render_scene(Canvas &canvas, Scene &scene,
                    unsigned short recursion_limit);

  void render_animation(Canvas &canvas, Scene &scene,
                        unsigned short recursion_limit,
                        unsigned short frame_count, std::string anim_name);

  Vector render_subpixel(Canvas &canvas, Scene &scene, int x, int y,
                         unsigned short recursion_limit);

  Vector CanvasToViewPort(Canvas &canvas, double x, double y);
};

#endif // !CAMERA_H
#define CAMERA_H
