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

// Screen
#define SCREEN_RESOLUTION 1000
#define SCREEN_WIDTH  SCREEN_RESOLUTION
#define SCREEN_HEIGHT SCREEN_RESOLUTION

// Math


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
    for (int iteration = 0; iteration < MAIN_ITERATIONS; ++iteration) {
        calculateScene(iteration);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    std::cout << "Elapsed time: " << duration.count() << " seconds" << std::endl;
    return 0;
}