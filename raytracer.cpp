#include "classes/Vector.h"

#include <array>
#include <fstream>
#include <iostream>
#include <limits>
#include <math.h>
#include <string>

using namespace std;

double INF = numeric_limits<double>::infinity();

class Canvas {
private:
  ofstream stream;
  string name;
  unsigned short width, height;

public:
  // Create function for rendering frames.

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

    stream << "P3 \n" << width << " " << height << "\n255\n"; // Header info.
  }

  ~Canvas() {
    if (stream.is_open())
      stream.close();
  }

  unsigned short getWidth() { return width; }
  unsigned short getHeight() { return height; }
  string getName() { return name; }

  void setName(string new_name) {
    stream.close();
    name = new_name;
    stream.open(new_name);

    stream << "P3 \n" << width << " " << height << "\n255\n";
  }

  void close() {
    if (stream.is_open())
      stream.close();
  }
  void open() {
    if (!stream.is_open())
      stream.open(name);
  }

  void operator<<(const string message) {
    if (!stream.is_open()) {
      stream.open(name);
    }

    stream << message;
  }

  void plot(Vector color) {
    stream << color.x << " " << color.y << " " << color.z << " \n";
  }
};

class Camera {
private:
  Vector origin;
  array<Vector, 3> rotational_matrix;

public:
  Camera() : origin(Vector()) { rotate(0, 0, 0); }
  Camera(Vector origin_vec) : origin(origin_vec) { rotate(0, 0, 0); }

  Vector getOrigin() { return origin; }

  void rotate(double yaw, double roll, double pitch) {
    yaw = yaw * M_PI / 180.0;
    roll = roll * M_PI / 180.0;
    pitch = pitch * M_PI / 180.0;

    array<Vector, 3> matrix = {
        {Vector(cos(roll) * cos(yaw),
                cos(roll) * sin(yaw) * sin(pitch) - sin(roll) * cos(pitch),
                cos(roll) * sin(yaw) * cos(pitch) + sin(roll) * sin(pitch)),
         Vector(sin(roll) * cos(yaw),
                sin(roll) * sin(yaw) * sin(pitch) + cos(roll) * cos(pitch),
                sin(roll) * sin(yaw) * cos(pitch) - cos(roll) * sin(pitch)),
         Vector(-sin(yaw), cos(yaw) * sin(pitch), cos(yaw) * cos(pitch))}};
    rotational_matrix = matrix;
  }

  void move(double displacement_x, double displacement_y,
            double displacement_z) {
    origin = origin + Vector(displacement_x, displacement_y, displacement_z);
  }

  void render_scene(Canvas &canvas, unsigned short recursion_limit);
  void render_animation(Canvas &canvas, unsigned short recursion_limit,
                        unsigned short frame_count, string anim_name);

  Vector CanvasToViewPort(double x, double y);
};
Vector operator*(array<Vector, 3> matrix, Vector vector) {
  return Vector(
      matrix[0].x * vector.x + matrix[1].x * vector.y + matrix[2].x * vector.z,
      matrix[0].y * vector.x + matrix[1].y * vector.y + matrix[2].y * vector.z,
      matrix[0].z * vector.x + matrix[1].z * vector.y + matrix[2].z * vector.z);
}

class Object3D {
public:
  virtual ~Object3D() {}

  virtual array<double, 2> intersect(Vector origin, Vector direction) const = 0;
  virtual Vector normal(Vector intersect_point) const = 0;
  virtual void shift(Vector displacement_vec) = 0;

  virtual Vector getCenter() const = 0;
  virtual Vector getColor() const = 0;
  virtual double getSpecular() const = 0;
  virtual double getReflective() const = 0;
};

class Sphere : public Object3D {
private:
  Vector center;
  double radius;
  Vector color;
  double specular;
  double reflective;
  bool is_valid;

public:
  Sphere() {
    center = Vector(0, 0, 0);
    radius = 0;
    color = Vector(0, 0, 0);
    specular = 0;
    reflective = 0;
    is_valid = false;
  }
  ~Sphere() {}

  Sphere(Vector center_val, double radius_val, Vector color_vec,
         float specular_val, float reflect_val) {
    center = center_val;
    radius = radius_val;
    color = color_vec;
    specular = specular_val;
    reflective = reflect_val;
    is_valid = true;
  }

  Vector getCenter() const { return center; }
  double getRadius() const { return radius; }
  Vector getColor() const { return color; }
  double getSpecular() const { return specular; }
  double getReflective() const { return reflective; }
  bool is_defined() const { return is_valid; }

  array<double, 2> intersect(Vector origin, Vector direction) const {
    Vector coord_offset = origin - center;

    double a = direction.dot(direction);
    double b = 2 * (coord_offset.dot(direction));
    double c = coord_offset.dot(coord_offset) - (radius * radius);

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
      const array<double, 2> result = {INF, INF};
      return result;
    }

    double t1 = (-b + sqrt(discriminant)) / (2 * a);
    double t2 = (-b - sqrt(discriminant)) / (2 * a);

    const array<double, 2> result = {t1, t2};
    return result;
  }

  Vector normal(Vector intersect_point) const {
    return (intersect_point - center) / radius;
  }

  void shift(Vector displacement_vec) { center = center + displacement_vec; }

  Sphere &operator=(const Sphere &other) {
    if (this != &other) { // Self-assignment check
      center = other.center;
      radius = other.radius;
      color = other.color;
      specular = other.specular;
      reflective = other.reflective;
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

class Plane : public Object3D {
private:
  Vector coefficients;
  Vector color;
  double position_constant;
  double specular;
  double reflective;

public:
  Plane()
      : coefficients(Vector()), color(Vector()), position_constant(0),
        specular(0) {}
  ~Plane() {}

  Plane(const char plane_type[2], Vector plane_color, double positional_const,
        double spec_value, double reflect_val) {
    unsigned int coeff_arr[3] = {0, 0, 0};

    // Only keep component that isn't in the plane_type.
    coeff_arr[0] = (plane_type[0] != 'x' && plane_type[1] != 'x');
    coeff_arr[1] = (plane_type[0] != 'y' && plane_type[1] != 'y');
    coeff_arr[2] = (plane_type[0] != 'z' && plane_type[1] != 'z');

    coefficients = Vector(coeff_arr[0], coeff_arr[1], coeff_arr[2]);
    color = plane_color;
    position_constant = positional_const;
    specular = spec_value;
    reflective = reflect_val;
  }

  Vector getCoefficients() const { return coefficients; }
  Vector getColor() const { return color; }
  double getPositionConstant() const { return position_constant; }
  double getSpecular() const { return specular; }
  double getReflective() const { return reflective; }
  Vector getCenter() const {
    return Vector(position_constant, position_constant, position_constant) *
           coefficients;
  }

  void shift(Vector displacement_vec) {
    double value;
    if (coefficients.x != 0)
      value = displacement_vec.x;
    else if (coefficients.y != 0)
      value = displacement_vec.y;
    else
      value = displacement_vec.z;

    position_constant += value;
  }

  array<double, 2> intersect(Vector origin, Vector direction) const {
    double D = -position_constant;

    double nominator = -(coefficients.dot(origin) + D);
    double denominator = coefficients.dot(direction);

    if (denominator == 0) {
      const array<double, 2> result = {INF, INF};
      return result;
    }

    double t1 = nominator / denominator;

    const array<double, 2> result = {t1, INF};
    return result;
  }

  Vector normal(Vector intersect_point) const {
    intersect_point.norm(); // supress unused parameter warning

    if (position_constant >= 0)
      return -coefficients;
    else
      return coefficients;
  }

  Plane &operator=(const Plane &other) {
    coefficients = other.coefficients;
    color = other.color;
    position_constant = other.position_constant;
    specular = other.specular;

    return *this;
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
  // array<Sphere, 4> objects;
  array<Light, 3> lights;
  array<Object3D *, 7> objects;

  Scene(string scene_identify) : scene_identification(scene_identify) {
    if (scene_identification == "basic") {
      array<Object3D *, 7> objects_arr = {
          {new Sphere(Vector(0, -1, 3), 1, Vector(255, 0, 0), 500, 0.2),
           new Sphere(Vector(2, 0, 4), 1, Vector(0, 255, 0), 500, 0.3),
           new Sphere(Vector(-2, 0, 4), 1, Vector(0, 0, 255), 500, 0.5),
           new Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 0), 1000,
                      0.1),
           new Sphere(), new Sphere(), new Sphere()}};
      objects = objects_arr;

      array<Light, 3> lights_arr = {
          {Light("ambient", 0.2), Light("point", 0.6, Vector(2, 1, 0)),
           Light("directional", 0.2, Vector(1, 4, 4))}};

      lights = lights_arr;
    } else if (scene_identification == "mirror") {
      array<Object3D *, 7> objects_arr = {{
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

      array<Light, 3> lights_arr = {{Light("ambient", 0.2),
                                     Light("point", 0.6, Vector(2, 5, -2)),
                                     Light()}};

      objects = objects_arr;
      lights = lights_arr;
    } else if (scene_identification == "animation") {
      array<Object3D *, 7> objects_arr = {
          {new Sphere(Vector(0, -5001, 0), 5000, Vector(255, 255, 255), 1000,
                      0.3), // white floor sphere
           new Sphere(Vector(-4, 0, 5), 1, Vector(255, 168, 167), 5,
                      0.3), // pink target sphere
           new Sphere(Vector(4, 0, 5), 1, Vector(168, 167, 255), 5, 0.3),
           new Sphere(), new Sphere(), new Sphere(), new Sphere()}};

      objects = objects_arr;

      array<Light, 3> lights_arr = {
          {Light("ambient", 0.1), Light("point", 0.2, Vector(2, 5, -2)),
           Light("directional", 0.6, Vector(0, 5, 0))}};

      lights = lights_arr;
    } else {
      array<Object3D *, 7> objects_arr = {{}};
      array<Light, 3> lights_arr = {{}};

      objects = objects_arr;
      lights = lights_arr;
    }
  }

  //~Scene() { for (size_t i = 0; i <= objects.size(); i++) { delete objects[i];
  //} }
};

// -------------- Environment Constants --------------

const double CANVAS_WIDTH = 500;
const double CANVAS_HEIGHT = 500;
const double VIEWPORT_WIDTH = 1;
const double VIEWPORT_HEIGHT = 1;
const double VIEWPORT_DISTANCE = 1;
const Vector BACKGROUND_COLOR(0, 0, 0);
const Scene scene("animation");

// ----------------------------------------------------

/*
Vector CanvasToViewPort(double x, double y) {
    return Vector(x * VIEWPORT_WIDTH/CANVAS_WIDTH, y *
VIEWPORT_HEIGHT/CANVAS_HEIGHT, VIEWPORT_DISTANCE);
}
*/

Vector Camera::CanvasToViewPort(double x, double y) {
  return rotational_matrix * Vector(x * VIEWPORT_WIDTH / CANVAS_WIDTH,
                                    y * VIEWPORT_HEIGHT / CANVAS_HEIGHT,
                                    VIEWPORT_DISTANCE);
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

pair<const Object3D *, double> ClosestIntersection(Vector origin,
                                                   Vector direction,
                                                   double t_min, double t_max) {
  double closest_t = INF;
  const Object3D *closest_object = nullptr;

  array<double, 2> t;
  for (unsigned short i = 0; i < scene.objects.size(); i++) {
    const Object3D *object = scene.objects[i];

    t = object->intersect(origin, direction);

    if ((t_min <= t[0] && t[0] <= t_max) && (t[0] < closest_t)) {
      closest_t = t[0];
      closest_object = scene.objects[i];
    }
    if ((t_min <= t[1] && t[1] <= t_max) && (t[1] < closest_t)) {
      closest_t = t[1];
      closest_object = scene.objects[i];
    }
  }

  return make_pair(closest_object, closest_t);
}

double ComputeLighting(Vector intersect_point, Vector normal_vec,
                       Vector view_vec, float spec_val) {

  Vector light_vec;
  Vector reflect_vec;
  double n_dot_l;
  double r_dot_v;
  double intense_cumm = 0.0;
  double t_max;

  for (unsigned int i = 0; i < scene.lights.size(); i++) {
    const Light &light = scene.lights[i];

    if (light.getType() == "ambient")
      intense_cumm += light.getIntensity();
    else {
      if (light.getType() == "point") {
        light_vec = light.getBehaviour() - intersect_point;
        t_max = 1;
      } else { // ambient
        light_vec = light.getBehaviour();
        t_max = INF;
      }

      pair<const Object3D *, double> result =
          ClosestIntersection(intersect_point, light_vec, 0.001, t_max);
      const Object3D *shadow_sphere = result.first;

      if (shadow_sphere != nullptr)
        continue;

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
    }
  }

  return intense_cumm;
}

Vector ReflectRay(Vector reflect_vec, Vector normal_vec) {
  return (normal_vec * 2 * normal_vec.dot(reflect_vec)) - reflect_vec;
}

Vector TraceRay(Vector origin, Vector direction, double t_min, double t_max,
                int recursion_depth) {
  pair<const Object3D *, double> result =
      ClosestIntersection(origin, direction, t_min, t_max);
  const Object3D *closest_object = result.first;
  double closest_t = result.second;

  if (closest_object == nullptr)
    return BACKGROUND_COLOR;

  Vector intersect_point = origin + (direction * closest_t);
  Vector normal_vec = closest_object->normal(intersect_point);
  normal_vec = normal_vec / normal_vec.norm();
  Vector view_vec = -direction;

  double intensity = ComputeLighting(intersect_point, normal_vec, view_vec,
                                     closest_object->getSpecular());
  Vector local_color = closest_object->getColor() * intensity;

  float reflect_val = closest_object->getReflective();

  if (recursion_depth <= 0 || reflect_val <= 0) {
    return local_color;
  }

  Vector reflection_vec = ReflectRay(view_vec, normal_vec);
  Vector reflected_color =
      TraceRay(intersect_point, reflection_vec, 0.01, INF, recursion_depth - 1);

  return (local_color * (1 - reflect_val)) + (reflected_color * reflect_val);
}

void Camera::render_scene(Canvas &canvas, unsigned short recursion_limit) {
  canvas.open();

  for (double y = floor(CANVAS_HEIGHT / 2) - 1; y >= -floor(CANVAS_HEIGHT / 2);
       y--) {
    for (double x = (-floor(CANVAS_WIDTH / 2)); x < floor(CANVAS_WIDTH / 2);
         x++) {
      Vector direction = CanvasToViewPort(x, y);
      Vector color = TraceRay(getOrigin(), direction, 1, INF, recursion_limit);

      color = ClampColor(color);
      canvas.plot(color);
    }
  }

  canvas.close();
}

double linear_map(double input, double input_min, double input_max,
                  double output_min, double output_max) {
  return output_min + ((output_max - output_min) / (input_max - input_min)) *
                          (input - input_min);
}

double fractional_function(double x, double strength) {
  return 1 / (pow(x, strength));
}

void Camera::render_animation(Canvas &canvas, unsigned short recursion_limit,
                              unsigned short frame_count, string anim_name) {
  string file_path = "animations/" + anim_name + "/";
  string canvas_name =
      canvas.getName().substr(0, canvas.getName().find(".ppm"));
  double temp;

  cout << "before: " << scene.objects[1]->getCenter() << endl;

  for (unsigned int i = 0; i < frame_count; i++) {
    canvas.setName(file_path + (canvas_name + to_string(i) + ".ppm"));
    render_scene(canvas, recursion_limit);

    temp = linear_map(i, 0, frame_count, 1, 3);
    if (anim_name == "roll") {
      cout << "movement: " << Vector(fractional_function(temp, 5), 0, 0)
           << endl;
      scene.objects[1]->shift(Vector(fractional_function(temp, 5), 0, 0));

    } else if (anim_name == "rise") {
      cout << "movement: " << Vector(0, fractional_function(temp, 5), 0)
           << endl;
      scene.objects[1]->shift(Vector(0, fractional_function(temp, 5), 0));

    } else if (anim_name == "collide") {
      cout << "before 1: " << scene.objects[1]->getCenter() << endl;
      cout << "before 2: " << scene.objects[2]->getCenter() << endl;

      scene.objects[1]->shift(Vector(fractional_function(temp, 4), 0, 0));
      scene.objects[2]->shift(Vector(-fractional_function(temp, 4), 0, 0));
    }

    cout << "after 1: " << scene.objects[1]->getCenter() << endl;
    cout << "after 2: " << scene.objects[2]->getCenter() << endl;
  }
}

int main() {
  Camera camera;
  // camera.move(1, 0, 0);
  // camera.rotate(-25, 15, -10);
  // camera.render_scene(canvas, recursion_limit);
  // this_thread::sleep_for(chrono::seconds(5));

  Canvas canvas("frame.ppm", CANVAS_HEIGHT, CANVAS_WIDTH);

  unsigned short recursion_limit = 5;
  camera.render_animation(canvas, recursion_limit, 30, "roll");

  return 1;
}
