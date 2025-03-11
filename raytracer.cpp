#include "classes/Vector.h"
#include "classes/Canvas.h"
#include "classes/Scene.h"
#include "classes/Inf.h"
#include "classes/Camera.h"

#include <array>
#include <math.h>

using namespace std;


// -------------- Environment Constants --------------

const double CANVAS_WIDTH = 500;
const double CANVAS_HEIGHT = 500;
const double VIEWPORT_WIDTH = 1;
const double VIEWPORT_HEIGHT = 1;
const double VIEWPORT_DISTANCE = 1;
const Vector BACKGROUND_COLOR(0, 0, 0);
const Scene scene("animation");

// ----------------------------------------------------

/*
Vector CanvasToViewPort(double x, double y) {
    return Vector(x * VIEWPORT_WIDTH/CANVAS_WIDTH, y *
VIEWPORT_HEIGHT/CANVAS_HEIGHT, VIEWPORT_DISTANCE);
}
*/

pair<const Object3D *, double> ClosestIntersection(Vector origin,
                                                   Vector direction,
                                                   double t_min, double t_max) {
  double closest_t = INF;
  const Object3D *closest_object = nullptr;

  array<double, 2> t;
  for (unsigned short i = 0; i < scene.objects.size(); i++) {
    const Object3D *object = scene.objects[i];

    t = object->intersect(origin, direction);

    if ((t_min <= t[0] && t[0] <= t_max) && (t[0] < closest_t)) {
      closest_t = t[0];
      closest_object = scene.objects[i];
    }
    if ((t_min <= t[1] && t[1] <= t_max) && (t[1] < closest_t)) {
      closest_t = t[1];
      closest_object = scene.objects[i];
    }
  }

  return make_pair(closest_object, closest_t);
}

double ComputeLighting(Vector intersect_point, Vector normal_vec,
                       Vector view_vec, float spec_val) {

  Vector light_vec;
  Vector reflect_vec;
  double n_dot_l;
  double r_dot_v;
  double intense_cumm = 0.0;
  double t_max;

  for (unsigned int i = 0; i < scene.lights.size(); i++) {
    const Light &light = scene.lights[i];

    if (light.getType() == "ambient")
      intense_cumm += light.getIntensity();
    else {
      if (light.getType() == "point") {
        light_vec = light.getBehaviour() - intersect_point;
        t_max = 1;
      } else { // ambient
        light_vec = light.getBehaviour();
        t_max = INF;
      }

      pair<const Object3D *, double> result =
          ClosestIntersection(intersect_point, light_vec, 0.001, t_max);
      const Object3D *shadow_sphere = result.first;

      if (shadow_sphere != nullptr)
        continue;

      n_dot_l = normal_vec.dot(light_vec);
      // Diffused
      if (n_dot_l > 0)
        intense_cumm += light.getIntensity() * n_dot_l /
                        (normal_vec.norm() * light_vec.norm());

      // Specular
      if (spec_val != -1) {
        reflect_vec = normal_vec * 2 * normal_vec.dot(light_vec) - light_vec;

        r_dot_v = reflect_vec.dot(view_vec);
        if (r_dot_v > 0)
          intense_cumm +=
              light.getIntensity() *
              pow((r_dot_v / (reflect_vec.norm() * view_vec.norm())), spec_val);
      }
    }
  }

  return intense_cumm;
}

Vector ReflectRay(Vector reflect_vec, Vector normal_vec) {
  return (normal_vec * 2 * normal_vec.dot(reflect_vec)) - reflect_vec;
}

Vector TraceRay(Vector origin, Vector direction, double t_min, double t_max,
                int recursion_depth) {
  pair<const Object3D *, double> result =
      ClosestIntersection(origin, direction, t_min, t_max);
  const Object3D *closest_object = result.first;
  double closest_t = result.second;

  if (closest_object == nullptr)
    return BACKGROUND_COLOR;

  Vector intersect_point = origin + (direction * closest_t);
  Vector normal_vec = closest_object->normal(intersect_point);
  normal_vec = normal_vec / normal_vec.norm();
  Vector view_vec = -direction;

  double intensity = ComputeLighting(intersect_point, normal_vec, view_vec,
                                     closest_object->getSpecular());
  Vector local_color = closest_object->getColor() * intensity;

  float reflect_val = closest_object->getReflective();

  if (recursion_depth <= 0 || reflect_val <= 0) {
    return local_color;
  }

  Vector reflection_vec = ReflectRay(view_vec, normal_vec);
  Vector reflected_color =
      TraceRay(intersect_point, reflection_vec, 0.01, INF, recursion_depth - 1);

  return (local_color * (1 - reflect_val)) + (reflected_color * reflect_val);
}

int main() {
  Camera camera;
  // camera.move(1, 0, 0);
  // camera.rotate(-25, 15, -10);
  // camera.render_scene(canvas, recursion_limit);
  // this_thread::sleep_for(chrono::seconds(5));

  Canvas canvas("frame.ppm", CANVAS_HEIGHT, CANVAS_WIDTH, VIEWPORT_HEIGHT, VIEWPORT_WIDTH, VIEWPORT_DISTANCE);

  unsigned short recursion_limit = 5;
  camera.render_animation(canvas, recursion_limit, 30, "roll");

  return 1;
}
