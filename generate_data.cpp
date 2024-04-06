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
    Constants, properties and variables
---------------------------------------- */
// Mandelbulb props
#define MAIN_ITERATIONS 500
double POWER_VAL = 8.0;
// Screen props
#define SCREEN_RESOLUTION 1000
#define SCREEN_WIDTH  SCREEN_RESOLUTION
#define SCREEN_HEIGHT SCREEN_RESOLUTION
// Math props
#define PI 3.141592653589793238462643
#define deg_2_rad(deg) (deg * PI/180.0)
// Raymarching props
#define SURFACE_DISTANCE 0.001
#define EPSILON   0.0001
#define MAX_STEPS 100
#define MAX_DISTANCE  5.0
// Camera props
double CAM_HORIZONTAL_ANGLE  = deg_2_rad(25.0);
double CAM_VERTICAL_ANGLE  = deg_2_rad(-15.0);
double CAM_FOV      = deg_2_rad(45);
double CAM_POSITION[3]   = {0.0, 0.0, 2.5}; 
double LIGHT_POSITION[3] = {2.0, 0.0, 5.0};
double TARGET_POSITION[3] = {0.0, 0.0, 0.0};
double UP_VECTOR[3] = {1.0, 0.0, 0.0};

/* ----------------------------------------
    Functions Declaration
---------------------------------------- */
double magnitude(double vector[]);
double mandelbulbDE(double rayPosition[]);
void addVectors(double vec1[], double vec2[], double result[]);
void subVectors(double vec1[], double vec2[], double result[]);
void normalizeVector(double vector[]);
double clamping(double val);
double dotProductVectors(double vec1[], double vec2[]);
void multiplyMatrixVector(double matrix[], double vector[], double result[]);
void copyVector(double vector[], double opy[]);
void rayMarching(int x, int y, double outputArray[]);
void moveVector(double vector[], double direction[], double distance, double result[]);
void crossVectors(double vec1[], double vec2[], double result[]);
double* viewMatrice();
void getNormal(double rayPosition[], double surfaceNormal[]);
void rotateVectorHorinzontally(double vector[], double angle);
void rotateVectorVertically(double vector[], double angle);
double getLight(double rayPosition[]);
void generateData(int currentIteration);
/* ----------------------------------------
    Functions Implementation
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
// Subtract two vectors
void subVectors(double vec1[], double vec2[], double result[]) {
    result[0] = vec1[0] - vec2[0];
    result[1] = vec1[1] - vec2[1];
    result[2] = vec1[2] - vec2[2];
}
// Normalize a vector
void normalizeVector(double vector[]) {
    double mag = magnitude(vector);
    if (mag == 0) { mag = 1;}
    vector[0] /= mag;
    vector[1] /= mag;
    vector[2] /= mag;
}
// Clamping function
double clamping(double value) {
    if (value >= 1.0) { return 1.0; }
    if (value <= 0.0) { return 0.0; }
    return value;
}
// Dot product of two vectors
double dotProductVectors(double vec1[], double vec2[]) {
    return (vec1[0]*vec2[0] + vec1[1]*vec2[1] + vec1[2]*vec2[2]);
}
// Multiply a matrix with a vector
void multiplyMatrixVector(double matrix[], double vector[], double result[]) {
    result[0] = matrix[0]*vector[0] + matrix[1]*vector[1] + matrix[2]*vector[2];
    result[1] = matrix[3]*vector[0] + matrix[4]*vector[1] + matrix[5]*vector[2];
    result[2] = matrix[6]*vector[0] + matrix[7]*vector[1] + matrix[8]*vector[2];
}
// Copy a vector
void copyVector(double vector[], double copy[]) {
    for (int i = 0; i < 3; ++i) { 
        copy[i] = vector[i]; 
    }
}
// Move a vector
void moveVector(double vector[], double direction[], double distance, double result[]) {
    result[0] = vector[0] + direction[0]*distance;
    result[1] = vector[1] + direction[1]*distance;
    result[2] = vector[2] + direction[2]*distance;
}
// Cross product of two vectors
void crossVectors(double vec1[], double vec2[], double result[]) {
    result[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    result[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    result[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}
// Calculate the view matrix
double* viewMatrice() {
    double vectorCamToTarget[3];
    subVectors(TARGET_POSITION, CAM_POSITION, vectorCamToTarget);
    normalizeVector(vectorCamToTarget);
    // Right basis vector
    double right_basis[3];
    crossVectors(vectorCamToTarget, UP_VECTOR, right_basis);
    normalizeVector(right_basis);
    // Up basis vector
    double up_basis[3];
    crossVectors(right_basis, vectorCamToTarget, up_basis);
    // Translate view matric to vector
    static double v_matrix[9] = {
        right_basis[0],  right_basis[1],  right_basis[2],
        up_basis[0],  up_basis[1],  up_basis[2],
        vectorCamToTarget[0], vectorCamToTarget[1], vectorCamToTarget[2]
    };
    return v_matrix;
}
// Get normal of a point
void getNormal(double rayPosition[], double surfaceNormal[]) {
    double x_epsi[3] = {EPSILON, 0, 0};
    double y_epsi[3] = {0, EPSILON, 0};
    double z_epsi[3] = {0, 0, EPSILON};
    double rayPositionWithEspilonX1[3];
    double rayPositionWithEspilonY1[3];
    double rayPositionWithEspilonZ1[3];
    addVectors(rayPosition, x_epsi, rayPositionWithEspilonX1);
    addVectors(rayPosition, y_epsi, rayPositionWithEspilonY1);
    addVectors(rayPosition, z_epsi, rayPositionWithEspilonZ1);
    double rayPositionWithEspilonX2[3];
    double rayPositionWithEspilonY2[3];
    double rayPositionWithEspilonZ2[3];
    subVectors(rayPosition, x_epsi, rayPositionWithEspilonX2);
    subVectors(rayPosition, y_epsi, rayPositionWithEspilonY2);
    subVectors(rayPosition, z_epsi, rayPositionWithEspilonZ2);
    surfaceNormal[0] = mandelbulbDE(rayPositionWithEspilonX1) - mandelbulbDE(rayPositionWithEspilonX2);
    surfaceNormal[1] = mandelbulbDE(rayPositionWithEspilonY1) - mandelbulbDE(rayPositionWithEspilonY2);
    surfaceNormal[2] = mandelbulbDE(rayPositionWithEspilonZ1) - mandelbulbDE(rayPositionWithEspilonZ2);
    normalizeVector(surfaceNormal);
} 
// Rotate a vector vertically
void rotateVectorVertically(double vector[], double angle) {
    double rotationMatrix[9] = {
        1.0, 0.0, 0.0, 
        0.0, cos(angle), -sin(angle),
        0.0, sin(angle), cos(angle)
    };
    double tempVector[3];
    multiplyMatrixVector(rotationMatrix, vector, tempVector);
    vector[0] = tempVector[0];
    vector[1] = tempVector[1];
    vector[2] = tempVector[2];
}
// Rotate a vector horizontally
void rotateVectorHorinzontally(double vector[], double angle) {
    double rotationMatrix[9] = {
        cos(angle), 0.0, sin(angle),
        0.0, 1.0, 0.0,
        -sin(angle), 0.0, cos(angle)
    };
    double tempVector[3];
    multiplyMatrixVector(rotationMatrix, vector, tempVector);
    vector[0] = tempVector[0];
    vector[1] = tempVector[1];
    vector[2] = tempVector[2];
}
// Get the light value of a point
double getLight(double rayPosition[]) {
    double lightPosition[3];
    // Get the light position
    copyVector(LIGHT_POSITION, lightPosition);
    // Apply rotation to the light position
    rotateVectorHorinzontally(lightPosition, CAM_HORIZONTAL_ANGLE);
    rotateVectorVertically(lightPosition, CAM_VERTICAL_ANGLE);
    // Get the light to surface angle
    double lightToSurfaceAngle[3];
    // Subtract the light position from the ray position
    subVectors(lightPosition, rayPosition, lightToSurfaceAngle);
    // Normalize the vector
    normalizeVector(lightToSurfaceAngle);
    double surfaceNormal[3];
    // Get the normal of the surface
    getNormal(rayPosition, surfaceNormal);
    // Return the dot product of the two vectors
    return clamping(dotProductVectors(surfaceNormal, lightToSurfaceAngle));
}

// Ray marching algorithm http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/
void rayMarching(int x, int y, double outputArray[]) {
    // Compute vector and coordinates from camera to object
    double distCameraObject = SCREEN_HEIGHT / tan(CAM_FOV);
        double vectorToPixel[3] = { (double)x, (double)y, 0.0 };
    double vectorToObj[3]   = { floor(SCREEN_WIDTH/2), floor(SCREEN_HEIGHT/2), distCameraObject };
    double worldCoordinates[3];
    subVectors(vectorToObj, vectorToPixel, worldCoordinates);
    normalizeVector(worldCoordinates);
    // Express coordinates in camera matrix view
    double cameraViewCoordinates[3];
    // Multiply the vector by the view matrix
    multiplyMatrixVector(viewMatrice(), worldCoordinates, cameraViewCoordinates);
    
    // Initialize ray travelled distance and position
    double rayDistance = 0; 
    double tempDistance;
    double rayBackStep[3];
    double cameraViewCoordinatesRotated[3];
    double rayPosition[3];
    // Output array init
    outputArray[0] = MAX_DISTANCE;
    outputArray[1] = 0;
    // Raymarching loop - Travelling in each direction
    for (int tempIteration = 0; tempIteration < MAX_STEPS; tempIteration++) {
        // For each step, move the ray in the direction of the camera
        moveVector(CAM_POSITION, cameraViewCoordinates, rayDistance, rayPosition);
        // Apply rotation to the ray
        rotateVectorVertically(rayPosition, CAM_VERTICAL_ANGLE);
        rotateVectorHorinzontally(rayPosition, CAM_HORIZONTAL_ANGLE);
        // Step ray distance with the mandelbulb distance estimator
        tempDistance = mandelbulbDE(rayPosition);
        rayDistance += tempDistance;
        // Save the distance and light value if close enough
        for (double mult = 2; mult < 4; ++mult) {
            // Check if the ray is close enough to the surface
            if (tempDistance < mult*SURFACE_DISTANCE) {
                // Apply rotation to the ray
                copyVector(cameraViewCoordinates, cameraViewCoordinatesRotated);
                rotateVectorVertically(cameraViewCoordinatesRotated, CAM_VERTICAL_ANGLE);
                rotateVectorHorinzontally(cameraViewCoordinatesRotated, CAM_HORIZONTAL_ANGLE);
                // Move the ray back a bit
                moveVector(rayPosition, cameraViewCoordinatesRotated, -EPSILON, rayBackStep);
                // Save the distance and light value
                outputArray[0] = MAX_DISTANCE - rayDistance;
                outputArray[1] = getLight(rayBackStep);
                break;
            }
        }
        if (tempDistance < SURFACE_DISTANCE) {
            // Apply rotation to the ray
            copyVector(cameraViewCoordinates, cameraViewCoordinatesRotated);
            rotateVectorVertically(cameraViewCoordinatesRotated, CAM_VERTICAL_ANGLE);
            rotateVectorHorinzontally(cameraViewCoordinatesRotated, CAM_HORIZONTAL_ANGLE);
            // Move the ray back a bit
            moveVector(rayPosition, cameraViewCoordinatesRotated, EPSILON, rayBackStep);
            // Save the distance and light value
            outputArray[0] = MAX_DISTANCE - rayDistance;
            outputArray[1] = getLight(rayBackStep);
            break;
        }
        // check if the ray is a lost cause (travelled too far)
        if (rayDistance > MAX_DISTANCE) { break; } 
    }
}
// Mandelbulb distance estimator
double mandelbulbDE(double rayPosition[]) {
    double dr = 1.0;
    double zr, r, teta, phi;
    double tempPosition[3];
    double cartesianPosition[3];
    copyVector(rayPosition, tempPosition);
    for (int tempIteration = 0; tempIteration < 15; tempIteration++) {
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
// Calculate the vizualisation data
void generateData(int currentIteration) {
    // Memory allocation according to the resolution
    double* distanceMatrice = new double[SCREEN_HEIGHT * SCREEN_WIDTH];
    double* lightMatrice = new double[SCREEN_HEIGHT * SCREEN_WIDTH];
    std::cout << "\t------ Calculating points for iteration :" << currentIteration << " ------" << std::endl;
    // Loop through the screen resolution
    double output[2];
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            rayMarching(x, y, output);
            distanceMatrice[y*SCREEN_WIDTH + x] = output[0];
            lightMatrice[y*SCREEN_WIDTH + x] = output[1];
        }
    }
    // Save the data to a file
    std::string strCurrentIteration = std::to_string(currentIteration);
    // Normalize string to 3 characters
    strCurrentIteration = std::string(3 - strCurrentIteration.length(), '0') + strCurrentIteration;
    
    // Save distance data
    std::cout << "\t> Saving distance file." << std::endl;
    std::ofstream distanceFile("data/distance_" + strCurrentIteration + ".txt");
    if (distanceFile.is_open()) {
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            for (int j = 0; j < SCREEN_HEIGHT; j++) {
                if (j < SCREEN_HEIGHT-1) { 
                    distanceFile << distanceMatrice[j*SCREEN_WIDTH + i] << ", "; 
                }
                else { 
                    distanceFile << distanceMatrice[j*SCREEN_WIDTH + i] << "\n"; 
                }
            }
        }
        distanceFile.close();
    } else {
        std::cout << "An error occurred writing distance file.\n";
        throw std::runtime_error("An error occurred writing distance file.\n");
    }

    // Save light data
    std::cout << "\t> Saving light file." << std::endl;
    std::ofstream lightFile("data/light_" + strCurrentIteration + ".txt");
    if (lightFile.is_open()) {
        for (int i = 0; i < SCREEN_WIDTH; i++) {
            for (int j = 0; j < SCREEN_HEIGHT; j++) {
                if (j < SCREEN_HEIGHT-1) { 
                    lightFile << lightMatrice[j*SCREEN_WIDTH + i] << ", "; 
                }
                else { 
                    lightFile << lightMatrice[j*SCREEN_WIDTH + i] << "\n"; 
                }
            }
        }
        lightFile.close();
    } else {
        std::cout << "An error occurred writing light file.\n";
        throw std::runtime_error("An error occurred writing light file.\n");
    }

    // Clean up memory
    delete[] distanceMatrice;
    delete[] lightMatrice;
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