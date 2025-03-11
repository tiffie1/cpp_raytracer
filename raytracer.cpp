#include "classes/Vector.h"
#include "classes/Canvas.h"
#include "classes/Scene.h"
#include "classes/Camera.h"


using namespace std;

int main() {
  const double CANVAS_WIDTH = 500;
  const double CANVAS_HEIGHT = 500;
  const double VIEWPORT_WIDTH = 1;
  const double VIEWPORT_HEIGHT = 1;
  const double VIEWPORT_DISTANCE = 1;
  const Vector BACKGROUND_COLOR(0, 0, 0);

  Scene scene("animation", BACKGROUND_COLOR);
  Canvas canvas("frame.ppm", CANVAS_HEIGHT, CANVAS_WIDTH, VIEWPORT_HEIGHT, VIEWPORT_WIDTH, VIEWPORT_DISTANCE);
  Camera camera;

  unsigned short recursion_limit = 5;
  camera.render_animation(canvas, scene, recursion_limit, 30, "roll");

  return 1;
}
