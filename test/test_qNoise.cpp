/*
qNoise: A generator of non-Gaussian colored noise
Copyright (c) 2015, Juan Ignacio Deza
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
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

   3. The names of its contributors may not be used to endorse or promote
      products derived from this software without specific prior written
      permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
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

