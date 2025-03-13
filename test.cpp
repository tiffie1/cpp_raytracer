#include <iostream>
// #include <string>

using namespace std;

double linear_map(double input, double input_min, double input_max,
                  double output_min, double output_max) {
  return output_min + ((output_max - output_min) / (input_max - input_min)) *
                          (input - input_min);
}

double fn(double x) { return 1 / (pow(x, 5)); }

int main() {
  float range = 0.1;
  float values[3] = {-range, 0, range};

  unsigned short iter = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      float off_x = values[i];
      float off_y = values[j];

      cout << "x: " << off_x << " " << "y: " << off_y << endl;
      cout << "iter: " << iter++ << endl << endl; // Action
    }
  }
}
