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

#ifndef QNOISEGEN_H
#define QNOISEGEN_H

#include <iostream>
#include <climits>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <random>

class qNoiseGen {
    double potQNoisePrime(double eta, double tau, double q);
    typedef std::chrono::high_resolution_clock myclock;
    myclock::time_point beginning;
    unsigned seed;
    std::mt19937 generator;
    std::normal_distribution<double> randNorm;
public:
    void seedManual(unsigned UserSeed);
    void seedTimer();
    double gaussWN();
    double orsUhl(double x, double tau, double H);
    double qNoise(double x, double tau, double q, double H, double sqrt_H);
    double qNoiseNorm(double x, double tau, double q, double H, double sqrt_H);

    qNoiseGen() {
        std::normal_distribution<double> randNorm(0.0, 1.0);
        beginning = myclock::now();
        seedTimer();
    }
};

#endif  // QNOISEGEN_H
