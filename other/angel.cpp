#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <utility>

using namespace std;

struct Vectores {

    double x, y, z;

    Vectores() : x(0), y(0), z(0) {}
    Vectores(double x, double y, double z) : x(x), y(y), z(z) {}

    Vectores operator+ (const Vectores& v) const {
        return Vectores(x + v.x, y + v.y, z + v.z);
    }

    Vectores operator- (const Vectores& v) const {
        return Vectores(x - v.x, y - v.y, z - v.z);
    }

    Vectores operator* (double s) const {
        return Vectores(x * s, y * s, z * s);
    }

    double dot (const Vectores& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vectores normalize() const {
        double mag = sqrt(x * x + y * y + z * z);
        return Vectores(x / mag, y / mag, z / mag);
    }
    
};

struct Sphere {
    Vectores Center;
    double Radius;
    Vectores Color;
    int Specular;
    double Reflective;

    Sphere(Vectores center, double radius, Vectores color, int specular, double reflective) : 
        Center(center), Radius(radius), Color(color), Specular(specular), Reflective(reflective) {}
};

struct Light {
    string Type;
    double Intensity;
    Vectores Position;
    Vectores Direction;

    Light(string type, double intensity, Vectores position, Vectores direction) :
        Type(type), Intensity(intensity), Position(position), Direction(direction) {}
};

vector<Sphere> spheres;
vector<Light> lights;


int clamp(double value, double min_val, double max_val) {
    return static_cast <int> (max(min_val, min(max_val, value)));
}

double length(const Vectores& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

bool IntersectRaySphere(const Vectores& O, const Vectores& D, const Sphere& sphere, double& t1, double& t2) {
    Vectores CO = O - sphere.Center;

    double a = D.dot(D);
    double b = 2 * CO.dot(D);
    double c = CO.dot(CO) - sphere.Radius * sphere.Radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;
    }

    t1 = (-b + sqrt(discriminant)) / (2 * a);
    t2 = (-b - sqrt(discriminant)) / (2 * a);

    return true;
}

pair<const Sphere*, double> ClosestIntersection(const Vectores& O, const Vectores& D, double t_min, double t_max) {
    double closest_t = INFINITY;
    const Sphere* closest_sphere = nullptr;

    for (const Sphere& sphere : spheres) {
        double t1, t2;

        if (IntersectRaySphere(O, D, sphere, t1, t2)) {
            if(t1 >= t_min && t1 <= t_max && t1 < closest_t) {
                closest_t = t1;
                closest_sphere = &sphere;
            }
            if(t2 >= t_min && t2 <= t_max && t2 < closest_t) {
                closest_t = t2;
                closest_sphere = &sphere;
            }
        }
    }

    return {closest_sphere, closest_t};
};

Vectores ReflectRay(const Vectores& R, const Vectores& N) {
    return  N * 2.0 * N.dot(R) - R;
}

double ComputeLighting(const Vectores& P, const Vectores& N, const Vectores& V, const int s) {
    double i = 0.0;
    Vectores L;
    double t_max;

    for (const Light& light : lights) {
        if (light.Type == "ambient") {
          i += light.Intensity ; 
        }
        else {
            if(light.Type == "point") {
               L =  (light.Position - P).normalize();
               t_max = length(light.Position - P);
            }
            else {
               L = light.Direction.normalize(); 
               t_max = INFINITY;
            }
            //Shadow Check
            pair<const Sphere*, double> shadow_sphere = ClosestIntersection(P, L, 0.001, t_max);

            if (shadow_sphere.first != nullptr) {
                continue;
            }
            //Diffuse
            double n_dot_l = N.dot(L);
            if(n_dot_l > 0) {
                i += light.Intensity * n_dot_l;
            }
            //Specular
            if (s != -1) {
                Vectores R = ReflectRay(N, L);
                double r_dot_v = R.dot(V);

                if( r_dot_v > 0) {
                    i += light.Intensity * pow(r_dot_v , s);
                }
            }
        }

    }
    return i;
}

Vectores TraceRay(const Vectores O, const Vectores D, double t_min, double t_max, int recursion_depth){
    
    pair<const Sphere*, double> intersection_result = ClosestIntersection(O, D, t_min, t_max);
    const Sphere* closest_sphere = intersection_result.first;
    double closest_t = intersection_result.second;

    if(closest_sphere == nullptr) {
        return Vectores(0, 0, 0); //Background Color
    }

    Vectores P = O + D * closest_t;
    Vectores N = (P - closest_sphere -> Center).normalize();
    
    Vectores V = D * -1.0;
    int s = closest_sphere -> Specular;
    
    double intensity = ComputeLighting(P, N, V, s);
    Vectores local_color = closest_sphere -> Color * intensity;
   
    double r = closest_sphere -> Reflective;
    if (recursion_depth <= 0 || r <= 0 ){
        return local_color;
    }
    Vectores R = ReflectRay(V, N);
    Vectores reflected_color = TraceRay(P, R, 0.001, INFINITY, recursion_depth - 1);

    return local_color * (1 - r) + reflected_color * r;
}

Vectores CanvasToViewport(int x, int y, int Cw, int Ch, int Vw, int Vh, int d) {
    return Vectores(
        (x + 0.5) * (double)Vw / Cw,  // Shift by 0.5 for better alignment
        (y + 0.5) * (double)Vh / Ch,
        d
    ).normalize();
}



int main(){
    int Cw = 500;
    int Ch = 500;
    int Vw = 1;
    int Vh = 1;
    int d = 1;

    int recursion_depth = 5;

    Vectores O(0, 0, 0);


    vector<Sphere> spheres_arr = {
        Sphere(Vectores(0, -1, 3), 1, Vectores(255, 0, 150), 0, 0.7), //rgb(255, 0, 150)
        Sphere(Vectores(-2, 1, 3), 1, Vectores(0, 255, 150), 0, 0.7),  //rgb(0, 255, 150)
        Sphere(Vectores(2, 1, 3), 1, Vectores(150, 100, 255), 0, 0.7), //rgb(150, 100, 255)
        Sphere(Vectores(0, -5001, 0), 5000, Vectores(255, 255, 0), 1000, 0.7)//rgb(241, 241, 90)
    };
    
    lights = vector<Light>{
        Light("ambient", 0.2, Vectores(), Vectores()),
        Light("point", 0.6, Vectores (2, 1, 0), Vectores()),
        Light("directional", 0.2, Vectores(), Vectores (1, 4, 4))
    };

    ofstream filePPM("all_ppm.ppm");
    filePPM << "P3\n" << Cw << " " << Ch << "\n255\n";

    for (double y = floor(Ch/2) - 1; y >= -floor(Ch/2); y--) {
        for (double x = (-floor(Cw / 2)); x < floor(Cw / 2); x++) {
            Vectores D = CanvasToViewport(x, y, Cw, Ch, Vw, Vh, d);
            Vectores Color = TraceRay(O, D, 1, INFINITY, recursion_depth);

            int red = clamp(Color.x, 0.0, 1.0) * 255.0;
            int green = clamp(Color.y, 0.0, 1.0) * 255.0;
            int blue = clamp(Color.z, 0.0, 1.0) * 255.0;

            filePPM << red << " " << green << " " << blue << " ";
        }
        filePPM << "\n";
    }
    cout << "PPM image generated" << endl;

    return 0;
}