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
  double temp;
  for (double i = 0; i < 10; i++) {
    temp = linear_map(i, 0, 10, 1, 3);
    cout << temp << endl;
    cout << fn(temp);
  }
}
