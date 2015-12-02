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
