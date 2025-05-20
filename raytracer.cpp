#include "classes/Camera.h"
#include "classes/Canvas.h"
#include "classes/Scene.h"
#include "classes/Vector.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main() {
  time_point<high_resolution_clock> start, end;
  microseconds duration;

  const double CANVAS_WIDTH = 1000;
  const double CANVAS_HEIGHT = 1000;
  const double VIEWPORT_WIDTH = 1;
  const double VIEWPORT_HEIGHT = 1;
  const double VIEWPORT_DISTANCE = 1;

  Camera camera;
  Scene scene(camera.getOrigin(), "chat", Vector(0, 0, 0));
  Canvas canvas("frame.ppm", CANVAS_HEIGHT, CANVAS_WIDTH, VIEWPORT_HEIGHT,
                VIEWPORT_WIDTH, VIEWPORT_DISTANCE);
  unsigned short recurr_lim = 10;

  camera.move(-4, -3, 2.5);
  camera.render_animation(canvas, scene, 10, 30, "passthru");

  /*
  cout << "Rendering starting:" << endl;
  for (unsigned short i = 0; i < 1; i++) {
    start = high_resolution_clock::now();

    camera.render_scene(canvas, scene, recurr_lim);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << static_cast<double>(duration.count()) / 1000000 << "," << endl;
  }
  */

  return 1;
}
