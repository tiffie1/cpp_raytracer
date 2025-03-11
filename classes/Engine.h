#ifndef ENGINE_H
#define ENGINE_H

#include "Object3D.h"
#include "Scene.h"
#include "Vector.h"

std::pair<const Object3D *, double>
ClosestIntersection(Scene &scene, Vector origin, Vector direction, double t_min,
                    double t_max);

double ComputeLighting(Scene &scene, Vector intersect_point, Vector normal_vec,
                       Vector view_vec, float spec_val);

Vector ReflectRay(Vector reflect_vec, Vector normal_vec);

Vector TraceRay(Scene &scene, Vector origin, Vector direction, double t_min,
                double t_max, int recursion_depth);

#endif // !ENGING_H
