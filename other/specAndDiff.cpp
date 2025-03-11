#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <string>

using namespace std;
double INF = numeric_limits<double>::infinity();

class Vector {
public:
  double x, y, z;

  Vector() {
    x = 0;
    y = 0;
    z = 0;
  }
  Vector(double x_val, double y_val, double z_val) {
    x = x_val;
    y = y_val;
    z = z_val;
  }

  Vector operator+(const Vector &other) {
    return Vector(x + other.x, y + other.y, z + other.z);
  }

  Vector operator-(const Vector &other) {
    return Vector(x - other.x, y - other.y, z - other.z);
  }

  Vector operator/(const Vector &other) {
    return Vector(x / other.x, y / other.y, z / other.z);
  }

  Vector operator*(const Vector &other) {
    return Vector(x * other.x, y * other.y, z * other.z);
  }

  Vector operator/(const double &scalar) {
    return Vector(x / scalar, y / scalar, z / scalar);
  }

  Vector operator*(const double &scalar) {
    return Vector(x * scalar, y * scalar, z * scalar);
  }

  Vector operator-() const { return Vector(-x, -y, -z); }

  double dot(const Vector &other) {
    return x * other.x + y * other.y + z * other.z;
  }
  double norm() { return sqrt(x * x + y * y + z * z); }

  /**
   * @brief Prints as (x, y, z).
   * @returns Stream object.
   */
  friend ostream &operator<<(ostream &stream, const Vector &vector) {
    stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return stream;
  }
};

class Canvas {
private:
  ofstream stream;
  string name;
  unsigned short width, height;

public:
  Canvas() {
    name = "";
    width = 0;
    height = 0;
  }
  Canvas(string file_name, unsigned short canvas_width,
         unsigned short canvas_height) {
    name = file_name;
    width = canvas_width;
    height = canvas_height;
    stream.open(name);

    stream << "P3 " << width << " " << height << "\n255\n"; // Header info.
  }
  ~Canvas() {
    if (stream.is_open())
      stream.close();
  }

  unsigned short getWidth() { return width; }
  unsigned short getHeight() { return height; }

  void operator<<(const string message) { stream << message; }

  void plot(Vector color) {
    stream << color.x << " " << color.y << " " << color.z << " ";
  }
};

class Sphere {
private:
  Vector center;
  double radius;
  Vector color;
  float specular;
  bool is_valid;

public:
  Sphere() {
    center = Vector(0, 0, 0);
    radius = 0;
    color = Vector(0, 0, 0);
    specular = 0;
    is_valid = false;
  }

  Sphere(Vector center_val, double radius_val, Vector color_vec,
         float specular_val) {
    center = center_val;
    radius = radius_val;
    color = color_vec;
    specular = specular_val;
    is_valid = true;
  }

  Vector getCenter() const { return center; }
  double getRadius() const { return radius; }
  Vector getColor() const { return color; }
  float getSpecular() const { return specular; }
  bool is_defined() { return is_valid; }

  const array<double, 2> intersect(Vector origin, Vector direction) const {
    Vector coord_offset = origin - center;

    double a = direction.dot(direction);
    double b = 2 * (coord_offset.dot(direction));
    double c = coord_offset.dot(coord_offset) - (radius * radius);

    double discriminant = b * b - 4 * a * c;

    // cout << "origin: " << origin << endl;
    // cout << "direction: " << direction << endl;

    if (discriminant < 0) {
      const array<double, 2> result = {INF, INF};
      return result;
    }

    double t1 = (-b + sqrt(discriminant)) / (2 * a);
    double t2 = (-b - sqrt(discriminant)) / (2 * a);

    const array<double, 2> result = {t1, t2};
    // cout << "t1 = " << t1 << "  t2 = " << t2;
    return result;
  }

  Vector normal(Vector intersect_point) const {
    return (intersect_point - center) / radius;
  }

  Sphere &operator=(const Sphere &other) {
    if (this != &other) { // Self-assignment check
      center = other.center;
      radius = other.radius;
      color = other.color;
      specular = other.specular;
      is_valid = true;
    }
    return *this;
  }

  friend ostream &operator<<(ostream &stream, const Sphere &sphere) {
    stream << "Center: " << sphere.center << "\nRadius: " << sphere.radius
           << "\nColor: " << sphere.color;

    return stream;
  }
};

class Light {
private:
  string type;
  double intensity;
  Vector behaviour;

public:
  Light() {
    type = "";
    intensity = 0;
    behaviour = Vector();
  }

  Light(string type_str, double intensity_val) {
    type = type_str;
    intensity = intensity_val;
  }

  Light(string type_str, double intensity_val, Vector behaviour_vec) {
    type = type_str;
    intensity = intensity_val;
    behaviour = behaviour_vec;
  }

  string getType() const { return type; }
  double getIntensity() const { return intensity; }
  Vector getBehaviour() const { return behaviour; }
};

class Scene {
private:
  string scene_identification;

public:
  array<Sphere, 4> objects;
  array<Light, 3> lights;

  Scene(std::string scene_identify) : scene_identification(scene_identify) {
    if (scene_identification == "basic") {
      array<Sphere, 4> objects_arr = {
          {Sphere(Vector(0, -1, 3), 1, Vector(255, 0, 0), 500),
           Sphere(Vector(2, 0, 4), 1, Vector(0, 255, 0), 500),
           Sphere(Vector(-2, 0, 4), 1, Vector(0, 0, 255), 500),
           Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 0), 1000)}};
      objects = objects_arr;

      array<Light, 3> lights_arr = {
          {Light("ambient", 0.2), Light("point", 0.6, Vector(2, 1, 0)),
           Light("directional", 0.2, Vector(1, 4, 4))}};

      lights = lights_arr;
    } else {
      array<Sphere, 4> objects_arr = {{}};
      array<Light, 3> lights_arr = {{}};

      objects = objects_arr;
      lights = lights_arr;
    }
  }
};

const double CANVAS_WIDTH = 500;
const double CANVAS_HEIGHT = 500;
const double VIEWPORT_WIDTH = 1;
const double VIEWPORT_HEIGHT = 1;
const double VIEWPORT_DISTANCE = 1;
const Vector BACKGROUND_COLOR(0, 0, 0);
const Vector ORIGIN(0, 0, 0);
const Scene scene("basic");

Vector CanvasToViewPort(double x, double y) {
  /*Vector result = Vector(x * VIEWPORT_WIDTH/CANVAS_WIDTH, y *
  VIEWPORT_HEIGHT/CANVAS_HEIGHT, VIEWPORT_DISTANCE);

  cout << "x: " << x << " y: " << y << "  ";
  cout << "viewport: " << VIEWPORT_WIDTH << " " << VIEWPORT_HEIGHT << "  ";
  cout << "canvas: " << CANVAS_WIDTH << " " << CANVAS_HEIGHT << "  ";
  cout << "Vw/Cw: " << (VIEWPORT_WIDTH/CANVAS_WIDTH) << "  ";
  cout << "result: " << result << endl;

  return result;*/
  return Vector(x * VIEWPORT_WIDTH / CANVAS_WIDTH,
                y * VIEWPORT_HEIGHT / CANVAS_HEIGHT, VIEWPORT_DISTANCE);
}

Vector ClampColor(Vector &color) {
  if (color.x < 0)
    color.x = 0;
  else if (color.x > 255)
    color.x = 255;
  if (color.y < 0)
    color.y = 0;
  else if (color.y > 255)
    color.y = 255;
  if (color.z < 0)
    color.z = 0;
  else if (color.z > 255)
    color.z = 255;

  return Vector(round(color.x), round(color.y), round(color.z));
}

double ComputeLighting(Vector intersect_point, Vector normal_vec,
                       Vector view_vec, double spec_val) {

  Vector light_vec;
  Vector reflect_vec;
  double n_dot_l;
  double r_dot_v;
  double intense_cumm = 0.0;

  for (unsigned int i = 0; i < scene.lights.size(); i++) {
    const Light &light = scene.lights[i];

    if (light.getType() == "ambient")
      intense_cumm += light.getIntensity();
    else {
      if (light.getType() == "point")
        light_vec = light.getBehaviour() - intersect_point;
      else // ambient
        light_vec = light.getBehaviour();

      n_dot_l = normal_vec.dot(light_vec);

      // Diffused
      if (n_dot_l > 0)
        intense_cumm += light.getIntensity() * n_dot_l /
                        (normal_vec.norm() * light_vec.norm());

      // Specular
      if (spec_val != -1) {
        reflect_vec =
            (normal_vec * 2) * (normal_vec.dot(light_vec)) - light_vec;

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

Vector TraceRay(Vector origin, Vector direction, double t_min, double t_max) {
  double closest_t = INF;
  const Sphere *closest_object = nullptr;

  array<double, 2> t;
  for (unsigned int i = 0; i < scene.objects.size(); i++) {
    const Sphere &object = scene.objects[i];

    t = object.intersect(origin, direction);

    if ((t_min <= t[0] && t[0] <= t_max) && (t[0] < closest_t)) {
      closest_t = t[0];
      closest_object = &scene.objects[i];
    }
    if ((t_min <= t[1] && t[1] <= t_max) && (t[1] < closest_t)) {
      closest_t = t[1];
      closest_object = &scene.objects[i];
    }
  }
  if (closest_object == nullptr)
    return BACKGROUND_COLOR;

  Vector intersect_point = origin + (direction * closest_t);
  Vector normal_vec = closest_object->normal(intersect_point);

  normal_vec = normal_vec / normal_vec.norm();
  Vector light_dir = -direction;
  double intensity = ComputeLighting(intersect_point, normal_vec, light_dir,
                                     closest_object->getSpecular());

  Vector color = closest_object->getColor();

  color = color * intensity;
  return ClampColor(color);
}

int main() {
  Canvas canvas("test.pbm", CANVAS_HEIGHT, CANVAS_WIDTH);

  for (double y = floor(CANVAS_HEIGHT / 2) - 1; y >= -floor(CANVAS_HEIGHT / 2);
       y--) {
    for (double x = (-floor(CANVAS_WIDTH / 2)); x < floor(CANVAS_WIDTH / 2);
         x++) {
      Vector direction = CanvasToViewPort(x, y);
      Vector color = TraceRay(ORIGIN, direction, 1, INF);
      canvas.plot(color);
    }
  }

  return 1;
}
