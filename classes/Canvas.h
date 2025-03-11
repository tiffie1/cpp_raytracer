#ifndef CANVAS_H
#define CANVAS_H

#include "Vector.h"
#include <fstream>
#include <string>

class Canvas {
private:
  std::ofstream stream;
  std::string name;
  unsigned short width, height;
  unsigned short v_width, v_height, v_dist;

public:
  Canvas();
  Canvas(std::string file_name, unsigned short canvas_width,
         unsigned short canvas_height, unsigned short viewport_width,
         unsigned short viewport_height, unsigned short viewport_distance);
  ~Canvas();

  unsigned short getWidth();
  unsigned short getHeight();
  unsigned short getV_Width();
  unsigned short getV_Height();
  unsigned short getV_Distance();
  std::string getName();
  void setName(std::string new_name);

  void close();
  void open();
  void plot(Vector color);

  void operator<<(const std::string message);
};

#endif // !CANVAS_H
