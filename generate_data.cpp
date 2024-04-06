#include <chrono>    // high_resolution_clock
#include <iostream>  // std::cout
#include <iomanip>   // setting precision of output strings
#include <sstream>   // converting numbers to strings streams
#include <string>    // std::string
#include <fstream>   // creating string streams
#include <algorithm> // std::clamp
#include <stdlib.h>  // srand, rand
#include <math.h>    // trig functions : sin, cos
#include <filesystem> // file system operations

/* ----------------------------------------
    Constants
---------------------------------------- */
// Mandelbulb props
double POWER_VAL = 8.0;
double TARGET_POSITION[3] = {0.0, 0.0, 0.0};
// Screen props
#define SCREEN_RESOLUTION 1000
#define SCREEN_WIDTH  SCREEN_RESOLUTION
#define SCREEN_HEIGHT SCREEN_RESOLUTION
// Math props
#define PI 3.141592653589793238462643
// Raymarching props
#define MAX_STEPS_NUM 100
#define MAIN_ITERATIONS 500

/* ----------------------------------------
    Function Declarations
---------------------------------------- */
double magnitude(double vector[]);
double mandelbulbDE(double rayPosition[]);
void addVectors(double vec1[], double vec2[], double result[]);
void copyVector(double vec[], double vec_copy[]);
/* ----------------------------------------
    Function Implementations
---------------------------------------- */
// Magnitude of a vector
double magnitude(double vector[]) {
    return sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
}
// Add two vectors
void addVectors(double vec1[], double vec2[], double result[]) {
    result[0] = vec1[0] + vec2[0];
    result[1] = vec1[1] + vec2[1];
    result[2] = vec1[2] + vec2[2];
}
// Copy a vector
void copyVector(double vector[], double copy[]) {
    for (int i = 0; i < 3; ++i) { 
        copy[i] = vector[i]; 
    }
}

// Mandelbulb distance estimator using ray marching algorithm http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/
double mandelbulbDE(double rayPosition[]) {
    double dr = 1.0;
    double zr, r, teta, phi;
    double tempPosition[3];
    double cartesianPosition[3];
    copyVector(rayPosition, tempPosition);
    for (int tmp_iter = 0; tmp_iter < 15; tmp_iter++) {
        // Break if the magnitude of the position vector is greater than 2
        r = magnitude(tempPosition);
        if (r > 2.0) { break; }
        // Estimate distance differential
        dr = POWER_VAL * pow(r, POWER_VAL-1.0) * dr + 1.0;
        // Convert to spherical coordinates
        teta = POWER_VAL * acos(tempPosition[2] / r);
        phi   = POWER_VAL * atan2(tempPosition[1], tempPosition[0]);
        zr    = pow(r, POWER_VAL);
        // Convert back to cartesian coordinates
        cartesianPosition[0] = zr * sin(teta) * cos(phi);
        cartesianPosition[1] = zr * sin(teta) * sin(phi);
        cartesianPosition[2] = zr * cos(teta);
        addVectors(rayPosition, cartesianPosition, tempPosition);
    }
    return 0.5*log(r)* r / dr;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Raymarching Mandelbulb Fractal..." << std::endl;
    std::cout << "\nClearing data directory\n";
    std::filesystem::path dataDir = "./data";
    for (const auto& entry : std::filesystem::directory_iterator(dataDir)) {
        std::filesystem::remove_all(entry.path());
    }
    std::cout << "Deleted files in data directory\n\n";
    std::cout << "> Total iterations: " << MAIN_ITERATIONS << std::endl;
    std::filesystem::create_directory("data");
    //todo
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    return 0;
}