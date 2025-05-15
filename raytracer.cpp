#include "classes/Camera.h"
#include "classes/Canvas.h"
#include "classes/Scene.h"
#include "classes/Vector.h"
#include <iostream>
#include <chrono>


using namespace std;
using namespace std::chrono;

int main() {
  auto start = high_resolution_clock::now();

  const double CANVAS_WIDTH = 500;
  const double CANVAS_HEIGHT = 500;
  const double VIEWPORT_WIDTH = 1;
  const double VIEWPORT_HEIGHT = 1;
  const double VIEWPORT_DISTANCE = 1;

  // Scene types:
  // - "basic", basic layout of three spheres.
  // - "mirror", two spheres inside of a room with mirrors in front of and directly behind the camera.
  // - "animation", simple layout with two spheres.
  Scene scene("basic", Vector(0, 0, 0));

  // Canvas name can be anything.
  Canvas canvas("frame.ppm", CANVAS_HEIGHT, CANVAS_WIDTH, VIEWPORT_HEIGHT,
                VIEWPORT_WIDTH, VIEWPORT_DISTANCE);
  Camera camera;
  unsigned short recurr_lim = 10;
  
  // There are three different animations supported.
  // "roll", "rise", and "collide".
  // All animations render the "animation" scene.
  // camera.render_animation(canvas, scene, recursion_limit, 30, "roll");

  camera.render_scene(canvas, scene, recurr_lim);
  auto end = high_resolution_clock::now();
  auto duration = duration_cast<milliseconds>(end - start);

  cout << "Scene rendered.\nTotal running time: " << static_cast<double>(duration.count())/1000 << "s." << endl;

  return 1;
}
