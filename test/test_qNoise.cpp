/*
qNoise: A generator of non-Gaussian colored noise
Copyright © 2015, Juan Ignacio Deza
email: juan.ignacio.deza@upc.edu

Description
qNoise is a non-gaussian colored random noise generator. It is a handy source of self-correlated noise for a great variety of applications. It depends on two parameters only: tau for controlling the autocorrelation, and q for controlling the statistics. This noise tends smoothly  for q = 1 to an  Ornstein-Uhlenbeck (colored gaussian) noise with autocorrelation tau. for q < 1 it is bounded noise and it is supra-Gaussian for q > 1.
The noise is generated  via a stochastic differential equation using the Heun method (a second order Runge-Kutta type integration scheme) and it is implemented as a stand-alone library in c++.
It Useful as input for numerical simulations, as a source of noise for controlling experiments using synthetic noise via micro-controllers and for a wide variety of applications.

Requirements
It is a stand-alone library with no dependencies other than the standard libraries.
Due to it's use of some functions from the <random> library the library currently works on c++11 or higher only.
This should be OK for most Macs and new Linux systems.
In some older systems it is possible that you need to add `-std=gnu++11` to your compilation flags.

Licence
 This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <cstdlib>
#include "../qNoise.h"
#include <sstream>
#include <fstream>

/*
 * This program shows the functionality of the class qNoise main function
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
    //Reads parameters from command line
    double tau = atof(argv[1]); //time constant
    double H = atof(argv[2]); //number of time steps per unit of time
    double q = atof(argv[3]); //parameter to switch statistics
    int N = atoi(argv[4]); //Number of points to generate
    double sqrt_H = sqrt(H); //defined for optimization purposes
    double x; //Value of the noise
    double temp_time = 2 * tau / H; //transient time. It's a good idea to make it dependent of Tau
    std::stringstream fileName; //Create filename
    fileName << "qNoise_" << tau << "_" << H << "_" << q << "_" << N << ".txt";
    std::ofstream myfile;
    myfile.open(fileName.str());
    //gen.seedManual(12345678); //Manual seeding (for debugging etc..), comment for auto seeding
    x = gen.gaussWN() / 100; //Initial value, can be anything. Better if it's around zero.
     for (int i = 0; i < N + temp_time; i++) {
        x = gen.qNoise(x, tau, q, H, sqrt_H);  //call to qNoise
        if (i > temp_time)
            myfile << x << std::endl;   //Write to disk after the transient.
    }
    myfile.close();
    if (verbose) std::cout << "Saved in file: " << fileName.str() << std::endl;
    return 0;
}

