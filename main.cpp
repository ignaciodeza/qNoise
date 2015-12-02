/* 
 * qNoise. 
 * Author: Juan Ignacio Deza
 */

#include <iostream>
#include <cstdlib>
#include "qNoise.h"
#include <sstream>
#include <fstream>

/*
 * This program Shows the functionality of the class qNoise main function
 * generating a file with a time series with the given parameters.
 * the program can be easily edited for other uses.
 */
int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Use: " << argv[0] << " tau H q N " << std::endl;
        return 1;
    }
    bool verbose = false;
    qNoiseGen gen;
    double tau = atof(argv[1]); //time constant 
    double H = atof(argv[2]); //number of time steps per unit of time
    double q = atof(argv[3]); //parameter to switch statistics
    int N = atoi(argv[4]); //parameter to switch statistics
    double sqrt_H = sqrt(H); //defined for optimization purposes
    double x; //Value of the noise
    double temp_time = 2 * tau / H; //transient time. 
    std::stringstream fileName; //Create filename
    fileName << "qNoise_" << tau << "_" << H << "_" << q << "_" << N << ".txt";
    std::ofstream myfile;
    myfile.open(fileName.str());
    //gen.seedManual(12345678); //Manual seeding (for debugging etc..), comment for auto seeding
    x = gen.gaussWN() / 100; //Initial value
     for (int i = 0; i < N + temp_time; i++) {
        x = gen.qNoiseNorm(x, tau, q, H, sqrt_H);
        if (i > temp_time)
            myfile << x << std::endl;
    }
    myfile.close();
    if (verbose) std::cout << "Saved in file: " << fileName.str() << std::endl;
    return 0;
}

