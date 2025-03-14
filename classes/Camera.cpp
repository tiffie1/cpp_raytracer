#include "Camera.h"
#include "Canvas.h"
#include "Engine.h"
#include "Inf.h"
#include "Scene.h"
#include "Vector.h"
#include <array>
#include <cmath>
#include <math.h>

Camera::Camera() : origin(Vector()) { rotate(0, 0, 0); }
Camera::Camera(Vector origin_vec) : origin(origin_vec) { rotate(0, 0, 0); }

Vector Camera::getOrigin() { return origin; }

void Camera::rotate(double yaw, double roll, double pitch) {
  yaw = yaw * M_PI / 180.0;
  roll = roll * M_PI / 180.0;
  pitch = pitch * M_PI / 180.0;

  std::array<Vector, 3> matrix = {
      {Vector(cos(roll) * cos(yaw),
              cos(roll) * sin(yaw) * sin(pitch) - sin(roll) * cos(pitch),
              cos(roll) * sin(yaw) * cos(pitch) + sin(roll) * sin(pitch)),
       Vector(sin(roll) * cos(yaw),
              sin(roll) * sin(yaw) * sin(pitch) + cos(roll) * cos(pitch),
              sin(roll) * sin(yaw) * cos(pitch) - cos(roll) * sin(pitch)),
       Vector(-sin(yaw), cos(yaw) * sin(pitch), cos(yaw) * cos(pitch))}};
  rotational_matrix = matrix;
}

void Camera::move(double displacement_x, double displacement_y,
                  double displacement_z) {
  origin = origin + Vector(displacement_x, displacement_y, displacement_z);
}

double linear_map(double input, double input_min, double input_max,
                  double output_min, double output_max) {
  return output_min + ((output_max - output_min) / (input_max - input_min)) *
                          (input - input_min);
}

double fractional_function(double x, double strength) {
  return 1 / (pow(x, strength));
}

Vector ClampColor(Vector &color) {
  if (color.x < 0)
    color.x = 0;
  else if (color.x > 255)
    color.x = 255;
  if (color.y < 0)
    color.y = 0;
  else if (color.y > 255)
    color.y = 255;
  if (color.z < 0)
    color.z = 0;
  else if (color.z > 255)
    color.z = 255;

  return Vector(round(color.x), round(color.y), round(color.z));
}

void Camera::render_scene(Canvas &canvas, Scene &scene,
                          unsigned short recursion_limit) {
  canvas.open();

  for (int y = floor(canvas.getHeight() / 2) - 1;
       y >= -floor(canvas.getHeight() / 2); y--) {
    for (int x = (-floor(canvas.getWidth() / 2));
         x < floor(canvas.getWidth() / 2); x++) {
      Vector color = render_subpixel(canvas, scene, x, y, recursion_limit);

      color = ClampColor(color);
      canvas.plot(color);
    }
  }

  canvas.close();
}

Vector Camera::render_subpixel(Canvas &canvas, Scene &scene, int x, int y,
                               unsigned short recursion_limit) {
  Vector colors[25];
  float range = 0.1;
  float off_x, off_y;
  float values[5] = {-range*2, -range, 0, range, range*2};
  unsigned short iter = 0;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      off_x = values[i];
      off_y = values[j];

      Vector direction = CanvasToViewPort(canvas, x + off_x, y + off_y);

      colors[iter++] =
          TraceRay(scene, origin, direction, 1, INF, recursion_limit);
    }
  }

  double r_acc = 0;
  double g_acc = 0;
  double b_acc = 0;

  for (int i = 0; i < 25; i++) {
    r_acc += colors[i].x;
    g_acc += colors[i].y;
    b_acc += colors[i].z;
  }

  // Mean
  double red = r_acc / 25;
  double green = g_acc / 25;
  double blue = b_acc / 25;

  return Vector(red, green, blue);
}

void Camera::render_animation(Canvas &canvas, Scene &scene,
                              unsigned short recursion_limit,
                              unsigned short frame_count,
                              std::string anim_name) {
  std::string file_path = "animations/" + anim_name + "/";
  std::string canvas_name =
      canvas.getName().substr(0, canvas.getName().find(".ppm"));
  double temp;

  for (unsigned int i = 0; i < frame_count; i++) {
    canvas.setName(file_path + (canvas_name + std::to_string(i) + ".ppm"));
    render_scene(canvas, scene, recursion_limit);

    temp = linear_map(i, 0, frame_count, 1, 3);
    if (anim_name == "roll") {
      scene.objects[1]->shift(Vector(fractional_function(temp, 5), 0, 0));

    } else if (anim_name == "rise") {
      scene.objects[1]->shift(Vector(0, fractional_function(temp, 5), 0));

    } else if (anim_name == "collide") {
      scene.objects[1]->shift(Vector(fractional_function(temp, 4), 0, 0));
      scene.objects[2]->shift(Vector(-fractional_function(temp, 4), 0, 0));
    }
  }
}

Vector operator*(std::array<Vector, 3> matrix, Vector vector) {
  return Vector(
      matrix[0].x * vector.x + matrix[1].x * vector.y + matrix[2].x * vector.z,
      matrix[0].y * vector.x + matrix[1].y * vector.y + matrix[2].y * vector.z,
      matrix[0].z * vector.x + matrix[1].z * vector.y + matrix[2].z * vector.z);
}

Vector Camera::CanvasToViewPort(Canvas &canvas, double x, double y) {
  return rotational_matrix *
         Vector(x * canvas.getV_Width() / canvas.getWidth(),
                y * canvas.getV_Height() / canvas.getHeight(),
                canvas.getV_Distance());
}
