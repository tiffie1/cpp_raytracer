#include <Canvas.h>

Canvas::Canvas() {
  name = "";
  width = 0;
  height = 0;
}

Canvas::Canvas(std::string file_name, unsigned short canvas_width,
               unsigned short canvas_height, unsigned short viewport_width,
               unsigned short viewport_height,
               unsigned short viewport_distance) {
  name = file_name;
  width = canvas_width;
  height = canvas_height;
  v_height = viewport_height;
  v_width = viewport_width;
  v_width = viewport_width;
  stream.open(name);

  stream << "P3 \n" << width << " " << height << "\n255\n"; // Header info.
}

Canvas::~Canvas() {
  if (stream.is_open())
    stream.close();
}

unsigned short Canvas::getWidth() { return width; }
unsigned short Canvas::getHeight() { return height; }
std::string Canvas::getName() { return name; }
unsigned short Canvas::getV_Width() { return v_width; }
unsigned short Canvas::getV_Height() { return v_height; }
unsigned short Canvas::getV_Distance() { return v_dist; }

void Canvas::setName(std::string new_name) {
  stream.close();
  name = new_name;
  stream.open(new_name);

  stream << "P3 \n" << width << " " << height << "\n255\n";
}

void Canvas::close() {
  if (stream.is_open())
    stream.close();
}
void Canvas::open() {
  if (!stream.is_open())
    stream.open(name);
}

void Canvas::operator<<(const std::string message) {
  if (!stream.is_open()) {
    stream.open(name);
  }

  stream << message;
}

void Canvas::plot(Vector color) {
  stream << color.x << " " << color.y << " " << color.z << " \n";
}
