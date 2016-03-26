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

#include"qNoise.h"

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

