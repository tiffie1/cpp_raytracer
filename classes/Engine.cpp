#include <Engine.h>
#include <Inf.h>
#include <Light.h>

std::pair<const Object3D *, double> ClosestIntersection(Scene &scene, Vector origin,
                                                   Vector direction,
                                                   double t_min, double t_max) {
  double closest_t = INF;
  const Object3D *closest_object = nullptr;

  std::array<double, 2> t;
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

  return std::make_pair(closest_object, closest_t);
}

double ComputeLighting(Scene &scene, Vector intersect_point, Vector normal_vec,
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

      std::pair<const Object3D *, double> result =
          ClosestIntersection(scene, intersect_point, light_vec, 0.001, t_max);
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

Vector TraceRay(Scene &scene, Vector origin, Vector direction, double t_min, double t_max,
                int recursion_depth) {
  std::pair<const Object3D *, double> result =
      ClosestIntersection(scene, origin, direction, t_min, t_max);
  const Object3D *closest_object = result.first;
  double closest_t = result.second;

  if (closest_object == nullptr)
    return scene.getBackgroundColor();

  Vector intersect_point = origin + (direction * closest_t);
  Vector normal_vec = closest_object->normal(intersect_point);
  normal_vec = normal_vec / normal_vec.norm();
  Vector view_vec = -direction;

  double intensity = ComputeLighting(scene, intersect_point, normal_vec, view_vec,
                                     closest_object->getSpecular());
  Vector local_color = closest_object->getColor() * intensity;

  float reflect_val = closest_object->getReflective();

  if (recursion_depth <= 0 || reflect_val <= 0) {
    return local_color;
  }

  Vector reflection_vec = ReflectRay(view_vec, normal_vec);
  Vector reflected_color =
      TraceRay(scene, intersect_point, reflection_vec, 0.01, INF, recursion_depth - 1);

  return (local_color * (1 - reflect_val)) + (reflected_color * reflect_val);
}
