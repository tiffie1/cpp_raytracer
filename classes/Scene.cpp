#include <Light.h>
#include <Plane.h>
#include <Scene.h>
#include <Sphere.h>
#include <Vector.h>

Scene::Scene(std::string scene_identify)
    : scene_identification(scene_identify) {
  if (scene_identification == "basic") {
    std::array<Object3D *, 7> objects_arr = {
        {new Sphere(Vector(0, -1, 3), 1, Vector(255, 0, 0), 500, 0.2),
         new Sphere(Vector(2, 0, 4), 1, Vector(0, 255, 0), 500, 0.3),
         new Sphere(Vector(-2, 0, 4), 1, Vector(0, 0, 255), 500, 0.5),
         new Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 0), 1000, 0.1),
         new Sphere(), new Sphere(), new Sphere()}};
    objects = objects_arr;

    std::array<Light, 3> lights_arr = {
        {Light("ambient", 0.2), Light("point", 0.6, Vector(2, 1, 0)),
         Light("directional", 0.2, Vector(1, 4, 4))}};

    lights = lights_arr;
  } else if (scene_identification == "mirror") {
    std::array<Object3D *, 7> objects_arr = {{
        new Plane("xz", Vector(255, 255, 255), -2, 300, 0),    // floor
        new Plane("xy", Vector(255, 255, 255), 5, 300, 1),     // front mirror
        new Plane("xy", Vector(255, 255, 255), -5, 300, 1),    // back mirror
        new Plane("yz", Vector(255, 255, 255), 5, 300, 0.01),  // right wall
        new Plane("yz", Vector(255, 255, 255), -5, 300, 0.01), // left wall
        new Sphere(Vector(-2, -1, 4), 1, Vector(0, 255, 255), 300,
                   0.2), // test subject
        new Sphere(Vector(2.4, -1.7, 3.6), 0.3, Vector(255, 0, 255), 500,
                   0.1) // other test subject
    }};

    std::array<Light, 3> lights_arr = {{Light("ambient", 0.2),
                                        Light("point", 0.6, Vector(2, 5, -2)),
                                        Light()}};

    objects = objects_arr;
    lights = lights_arr;
  } else if (scene_identification == "animation") {
    std::array<Object3D *, 7> objects_arr = {
        {new Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 255), 1000,
                    0.3), // white floor sphere
         new Sphere(Vector(-4, 0, 5), 1, Vector(255, 168, 167), 5,
                    0.3), // pink target sphere
         new Sphere(Vector(4, 0, 5), 1, Vector(168, 167, 255), 5, 0.3),
         new Sphere(), new Sphere(), new Sphere(), new Sphere()}};

    objects = objects_arr;

    std::array<Light, 3> lights_arr = {
        {Light("ambient", 0.1), Light("point", 0.2, Vector(2, 5, -2)),
         Light("directional", 0.6, Vector(0, 5, 0))}};

    lights = lights_arr;
  } else {
    std::array<Object3D *, 7> objects_arr = {{}};
    std::array<Light, 3> lights_arr = {{}};

    objects = objects_arr;
    lights = lights_arr;
  }
}
