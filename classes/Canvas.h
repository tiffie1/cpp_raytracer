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

public:
  Canvas();
  Canvas(std::string file_name, unsigned short canvas_width,
         unsigned short canvas_height);
  ~Canvas();

  unsigned short getWidth();
  unsigned short getHeight();
  std::string getName();
  void setName(std::string new_name);

  void close();
  void open();
  void plot(Vector color);

  void operator<<(const std::string message);
};

#endif // !CANVAS_H
