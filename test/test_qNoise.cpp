/*
qNoise: A generator of non-Gaussian colored noise
Copyright (c) 2021-2025 J. Ignacio Deza

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

Description:
qNoise is a non-Gaussian colored random noise generator. It is a handy source 
of self-correlated noise for a great variety of applications. It depends on 
two parameters only: tau for controlling the autocorrelation, and q for 
controlling the statistics.

- q = 1: Ornstein-Uhlenbeck (colored Gaussian) noise
- q < 1: Bounded noise (sub-Gaussian)
- q > 1: Heavy-tailed noise (supra-Gaussian)

The noise is generated via a stochastic differential equation using the Heun 
method (a second-order Runge-Kutta integration scheme).

Requirements:
- C++11 or higher (uses <random> library)
- No external dependencies

Contact: ignacio.deza@uwe.ac.uk

Citation:
Deza, J.I., Ihshaish, H. (2022). qNoise: A generator of non-Gaussian colored 
noise. SoftwareX, 18, 101034. https://doi.org/10.1016/j.softx.2022.101034
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

