#include "Engine.h"
#include "Inf.h"
#include "Light.h"
#include "Scene.h"
#include <cstdlib>
#include <iostream>

std::pair<const Sphere *, double>
ClosestIntersection(Scene &scene, Vector origin, Vector direction, double t_min,
                    double t_max) {
  double closest_t = INF;
  const Sphere *closest_object = nullptr;

  std::array<double, 2> t;
  double direction_dot = direction.dot(direction);

  for (unsigned short i = 0; i < scene.objects.size(); i++) {
    const Sphere *object = scene.objects[i];

    t = object->intersect(origin, direction, direction_dot);

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

inline void calculate_spec_diff(Scene &scene, const Light &light,
                                Vector &intersect_point, Vector &light_vec,
                                Vector &normal_vec, Vector &view_vec,
                                double &t_max, float &spec_val,
                                double &intense_cumm) {

  double n_dot_l;
  double r_dot_v;
  Vector reflect_vec;

  //  Trace ray from point of object intersection to light direction.
  std::pair<const Sphere *, double> result =
      ClosestIntersection(scene, intersect_point, light_vec, 0.001, t_max);
  const Sphere *shadow_sphere = result.first;

  // There is a block for the light, and skip diff and spec.
  if (shadow_sphere != nullptr) {
    scene.recently_shaded->point = intersect_point;
    scene.recently_shaded->light_dir = light_vec;
    scene.recently_shaded->hit_shadow = true;
  } else {
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

    scene.recently_shaded->point = intersect_point;
    scene.recently_shaded->light_dir = light_vec;
    scene.recently_shaded->hit_shadow = false;
  }
}

inline Vector normalize(Vector &v) {
  double len = v.norm();
  return (len > 0.0) ? (v / len) : Vector(0, 0, 0);
}

double EPSILON = 0.0001;

double ComputeLighting(Scene &scene, Vector intersect_point, Vector normal_vec,
                       Vector view_vec, float spec_val) {
  Vector light_vec;
  Vector reflect_vec;
  double n_dot_l;
  double r_dot_v;
  double intense_cumm = 0.0;
  double t_max;
  // bool shadow_hit_cached;

  // For every light source.
  for (unsigned int i = 0; i < scene.lights.size(); i++) {
    const Light &light = scene.lights[i];

    if (light.getType() == "ambient") {
      intense_cumm += light.getIntensity();
      continue;
    } else {
      if (light.getType() == "point") {
        light_vec = light.getBehaviour() - intersect_point;
        t_max = 1;
      } else { // directional
        light_vec = light.getBehaviour();
        t_max = INF;
      }

      calculate_spec_diff(scene, light, intersect_point, light_vec,
                          normal_vec, view_vec, t_max, spec_val,
                          intense_cumm);

      /*
      Vector L1 = normalize(light_vec);
      Vector L0 = normalize(scene.recently_shaded->light_dir);
      double angle_cos = L1.dot(L0);

      if (((scene.recently_shaded->point - intersect_point).norm() <=
           EPSILON) &&
          (angle_cos) <= 0.9999) {
        if (scene.recently_shaded->hit_shadow)
          continue;
        else
          calculate_spec_diff(scene, light, intersect_point, light_vec,
                              normal_vec, view_vec, t_max, spec_val,
                              intense_cumm);
      } else {
        //std::cout << "Not closeby." << std::endl;
        calculate_spec_diff(scene, light, intersect_point, light_vec,
                            normal_vec, view_vec, t_max, spec_val,
                            intense_cumm);
      }
      */
    }
  }

  return intense_cumm;
}

Vector ReflectRay(Vector reflect_vec, Vector normal_vec) {
  return (normal_vec * 2 * normal_vec.dot(reflect_vec)) - reflect_vec;
}

bool Refract(Vector &incident, Vector &normal, double ior, Vector &refracted) {
  double cosi = std::clamp(-1.0, 1.0, incident.dot(normal));
  double etai = 1.0, etat = ior;
  Vector n = normal;

  if (cosi < 0) { // Ray is entering the object
    cosi = -cosi;
  } else { // Ray is exiting the object
    std::swap(etai, etat);
    n = -normal;
  }

  double eta = etai / etat;
  double k = 1 - eta * eta * (1 - cosi * cosi);

  if (k < 0) {
    return false; // Total internal reflection
  } else {
    refracted = incident * eta + n * (eta * cosi - sqrt(k));
    // refracted.y = -refracted.y;
    return true;
  }
}

Vector TraceRay(Scene &scene, Vector origin, Vector direction, double t_min,
                double t_max, int recursion_depth) {
  std::pair<const Sphere *, double> result =
      ClosestIntersection(scene, origin, direction, t_min, t_max);

  const Sphere *closest_object = result.first;
  double closest_t = result.second;

  if (closest_object == nullptr)
    return scene.getBackgroundColor();

  Vector intersect_point = origin + (direction * closest_t);
  Vector normal_vec = closest_object->normal(intersect_point);
  normal_vec = normal_vec / normal_vec.norm();
  Vector view_vec = -direction;

  double intensity = ComputeLighting(scene, intersect_point, normal_vec,
                                     view_vec, closest_object->getSpecular());
  Vector local_color = closest_object->getColor() * intensity;

  float reflect_val = closest_object->getReflective();
  double transparency = closest_object->getTransparency();
  double ior = closest_object->getRefractiveIdx();

  if (recursion_depth <= 0) {
    return local_color;
  }

  Vector result_color = local_color;
  Vector reflected_color;

  if (reflect_val > 0) {
    Vector reflection_vec = ReflectRay(view_vec, normal_vec);
    reflected_color = TraceRay(scene, intersect_point, reflection_vec, 0.001,
                               INF, recursion_depth - 1);

    result_color =
        result_color * (1 - reflect_val) + reflected_color * reflect_val;
  }

  if (transparency > 0) {
    Vector refracted_dir;
    bool success = Refract(direction, normal_vec, ior, refracted_dir);

    if (success) {
      Vector refracted_color = TraceRay(scene, intersect_point, refracted_dir,
                                        0.001, INF, recursion_depth - 1);
      result_color =
          result_color * (1 - transparency) + refracted_color * transparency;
    }
  }

  return result_color;
}
