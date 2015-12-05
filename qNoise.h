// qNoise: A generator of non-Gaussian colored noise
// Juan Ignacio Deza. 4 dic 2015. juan.ignacio.deza@upc.edu

//We introduce a software generator for a class of colored (self-correlated) and
//non-Gaussian noise, whose statistics and spectrum depend upon only two parameters. 
//Inspired by Tsallis’ nonextensive formulation of statistical physics, this 
//so-called q-distribution is a handy source of auto-correlated noise for a great 
//variety of applications. This noise has a smooth limit for q = 1 to an Ornstein– 
//Uhlenbeck noise of autocorrelation τ. The noise is generated via a stochastic 
//differential equation using a second order Runge-Kutta integration scheme 
//(also known as the Heun method) and it is implemented as a stand alone library 
//in the language c++ available with an open source licence in the repository github[1]. 
//Their statistics can be chosen at will, by varying a single parameter q: 
//it has compact support for q < 1, and its variance tends to infinity as q 
//approaches q = 5/3. The software here introduced focuses in the regime departures 
//from Gaussianity, namely 0 < q < 5/3 where the variance is still normalizable. 
//Furthermore. The autocorrelation of the noise can be tuned—by means of parameter 
//τ—independently from the statistics providing a tool to examine the effects of 
//non-Gaussian Colored noise, as a way to model many real-world types of noise. 
//Applications are discussed, illustrating how the noise statistics affects the 
//response of a variety of nonlinear systems. In many of these examples, the system’s 
//response turns out to be optimal for some q 􏰀 1. This software is thus suitable 
//for the simulation of a wide range of types of noise, or study the effect of 
//correlation and deviations from Gaussianity in Stochastic ODEs, This is relevant 
//for a wide variety of technological applications, as well as for the understanding 
//of situations of biological interest.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//   1. Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//   3. The names of its contributors may not be used to endorse or promote 
//      products derived from this software without specific prior written 
//      permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


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
    double qNoise(double eta, double tau, double q, double H, double sqrt_H);
    double qNoiseNorm(double eta, double tau, double q, double H, double sqrt_H);

    qNoiseGen() {
        std::normal_distribution<double> randNorm(0.0, 1.0);
        beginning = myclock::now();
        seedTimer();
    }
};

//Manual seeding.

void qNoiseGen::seedManual(unsigned UserSeed) {
    seed = UserSeed;
    generator.seed(seed);
}

//Timer seeding
//Note: when using this on multiple threads, call seedManual on each thread using different values of seed

void qNoiseGen::seedTimer() {
    // obtain a seed from the timer
    myclock::duration d = myclock::now() - beginning;
    seed = d.count();
    generator.seed(seed);

}

//Gaussian White noise

double qNoiseGen::gaussWN() {
    return randNorm(generator);
}

//Ornstein-Uhlembeck noise type.

double qNoiseGen::orsUhl(double eta, double tau, double H) {
    return eta * exp(-H / tau) + sqrt((1 - exp(-2 / tau * H)) / 2 / tau) * randNorm(generator);
}

/*
 * qNoise Potential derivative. Private Function for use only by qNoise
 * The potential is:
 * \frac{\text{sigma}^2 \log \left(\frac{\text{eta}^2 (q-1) \text{tau}}{\text{sigma}^2}+1\right)}{2 (q-1) \text{tau}}
 * it's derivative is:
 * \frac{\text{eta}}{\frac{\text{eta}^2 (q-1) \text{tau}}{\text{sigma}^2}+1}
 * The /tau is the tau present in the differential equation.
 */
double qNoiseGen::potQNoisePrime(double eta, double tau, double q) {
    return (eta / (1 + (eta * eta * tau * (q - 1)))) / tau;
}

/*qNoise.
 * This functions integrates a differential equation using the Heun Method
 * for q=1 it behaves like the orstein Uhlembeck noise
 * for q<1 it it is defined in a acotated support only (+/- etaCut)
 * for q>1 its statistics are more than gaussian tending (supra-gaussian)
 */
double qNoiseGen::qNoise(double eta, double tau, double q, double H, double sqrt_H = -1) {
    double kHeun, lHeun, differential;
    bool error = false;
    int countError = 0;
    //If the square root of H is provided, it will be used, otherwise
    //calculate it every time the function is invoked.
    if (sqrt_H < 0)
        sqrt_H = sqrt(H);
    //The cut value.
    double etaCut = 1 / sqrt(tau * (1 - q));
    while (1) {
        kHeun = H * potQNoisePrime(eta, tau, q);
        lHeun = sqrt_H * randNorm(generator) / tau;
        differential = -H / 2 * (potQNoisePrime(eta, tau, q)
                + potQNoisePrime(eta + kHeun + lHeun, tau, q)) + lHeun;
        /*
         * Check if the system is inside the boundary.
         * This is only important when q<1.
         * If the eta + D_eta are outside the bounds, it retries 10 times.
         * If it is still out of bounds, it retries with an OH noise, another 10 times
         * This very time step will be Gaussian but with the correct tau.
         * If it is still out of bounds it hard resets it with gaussian noise.
         * The bool error is on place to debug these errors.
         * However it should be a concern only for very low values of q and very 
         * hgih values of tau.
         */
        if ((fabs(eta + differential) > etaCut) || isnan(eta + differential)) {
            countError++;
            if (countError > 20) {
                return etaCut * randNorm(generator);
                if (error) std::cerr << "Out of bounds phase 3: " << eta << "\t" << differential << std::endl;
            }
            if (countError > 10) {
                eta = etaCut * orsUhl(eta, tau, H);
                if (error) std::cerr << "Out of bounds phase 2: " << eta << "\t" << differential << std::endl;
            } else {
                if (error) std::cerr << "Out of bounds: " << eta << "\t" << differential << std::endl;
            }
        } else {
            return eta + differential;
        }
    }
}

/*qNoiseNorm.
 * This functions works as qNoise but where both tau and the variance of the noise 
 * are independent of q to first order.
 * This approximation fails for q -> 5/3, where both variables diverge.
 */
double qNoiseGen::qNoiseNorm(double eta, double tau, double q, double H, double sqrt_H = -1) {
    return qNoise(eta, tau * (5 - 3 * q) / 2, q, H, sqrt_H);
}

#endif  // QNOISEGEN_H
