# qNoise: A Generator of non-Gaussian Colored Noise

## Description

**qNoise** is a non-gaussian colored random noise generator. It is a handy source of *self-correlated* noise for a great variety of applications. It depends on two parameters only: **tau** for controlling the autocorrelation, and **q** for controlling the statistics. This noise tends smoothly  for *q = 1* to an  *Ornstein-Uhlenbeck* (colored gaussian) noise with autocorrelation *tau*. for *q < 1* it is **bounded noise** and it is **supra-Gaussian** for *q > 1*.

The noise is generated  via a stochastic differential equation using the Heun method (a second order Runge-Kutta type integration scheme) and it is implemented as a stand-alone library in c++.

It is useful as input for numerical simulations, as a source of noise for controlling experiments using synthetic noise via micro-controllers and for a wide variety of applications.

## Requirements

It is stand-alone with no dependencies other than the standard libraries.
Due to it's use of some functions from the **\<random\>** library, it currently works on *c++11 or higher* **only**.

This should be OK for most Macs and new Linux/Unix systems. On Windows please ensure you upgrade to a c++11 compatible compiler, for example **GNU g++ 4.7 or higher**.

In some older systems it is possible that you need to add `-std=gnu++11` to your compilation flags.

## Installation

#### Short story:
Type the following commands on a terminal. The explanations come below.

```
git clone https://github.com/ignaciodeza/qNoise.git
cd qNoise/test
make test
./runTest.sh

```
If you see
```
All tests OK.
```
at the end of the process then **qNoise** is installed and works in your system, if you don't want to know more about the installation, you can go directly to **Usage**.

#### Long story:

If you haven't done this yet, clone the repository by typing on a terminal window
```
git clone https://github.com/ignaciodeza/qNoise.git
```
To compile the test, from the terminal go to the folder test inside the cloned directory by typing:
```
cd qNoise/test
```
Compile the test, using the qNoise library
```
make test
```
and run the battery of tests.
```
./runTest.sh
```
In less than a second, examples of several types of noise will be shown in the terminal. The files will be saved on disk.
You should see several sets of parameters and in the last line:
```
All tests OK.
```
This means the software is installed and works in your system.

For cleaning the files and the executables from the test folder, simply type
```
make clean
```

## Usage

#### Source Code
Just move both `qNoise.h` and `qNoise.cpp` to the folder where your code is. No other installation is necessary.
Include as `#include"qNoise.h"`in your projects.

In some older systems is possible that you need to add `-std=gnu++11` to your compilation flags.

##### Contents
The library provides four methods.

A generator of Gaussian white noise (for convenience).
```
double gaussWN();
```
A generator of Gaussian Colored noise (Orstein-Uhlenbeck)
```
double orsUhl(double x, double tau, double H);
```
A generator of qNoise, valid in the whole range, where *tau* and the variance of the noise diverge for *q = 5/3*
```
double qNoise(double x, double tau, double q, double H, double sqrt_H);
```
A normalized version of the former, where *tau* and the variance are independent of q, valid sufficiently far away from *q = 5/3*
```
double qNoiseNorm(double x, double tau, double q, double H, double sqrt_H);
```
Additionally it provides two methods for seeding the random number generator, manually and using the timer:
```
void seedManual(unsigned Seed);
void seedTimer();
```
The timer is used by default.

#### Executable
The executable `test_qNoise` can be also used as a standalone noise generator.

It accepts four commands
```
./test_qNoise tau H q N
```
- **tau** is the autocorrelation
- **H** is the integration step
- **q** controls the statistics
- **N** is the number of points to calculate

For example (try it!):

```
./test_qNoise 1 0.01 1.2 1000
```
will generate 1000 points of supra-Gaussian noise (q=1.2) of tau=1 (relative to H=0.01 )
and put them into the file
```
qNoise_1_0.01_1.2_1000.txt
```
which explicitly contains all the parameters.

This file together with the `runTest.sh` can be edited to better suit your needs and compiled using the same scheme provided above.

## Unit Test

A set of unit tests is provided. It is a rather stern test which compares the histograms to the theoretical PDFs. It calculates the square root of sum the square of the difference between both functions as a percentage of the maximum height of the distribution. 
This allows to have a metric of the "goodness" of the histogram of the generated noise. Note that it may take time (N) for the histogram to take the shape of the PDF, and this time may be considerably higher for highly correlated noise. 

In order to access to this unit test first uncomment the following line in `qNoise.cpp`:

```
// #define UNIT_TEST
#ifdef UNIT_TEST
#include "unit_tests.cpp"
#endif
```

And then compile and run, with a command similar to this (it may change in your system):

```
c++ -std=gnu++11 qNoise.cpp -o qNoise; qNoise
```

The limit of 10% of difference for passing the test is arbitrary. But I found it to be a good guideline for most applications.

## References

Please read the following paper which describes the software and it´s applications
https://www.sciencedirect.com/science/article/pii/S2352711022000346

## Citing

If you use this software please cite us! Bibtex entry below

```
@article{deza2022qnoise,
  title={qNoise: A generator of non-Gaussian colored noise},
  author={Deza, J. Ignacio and Ihshaish, Hisham},
  journal={SoftwareX},
  volume={18},
  pages={101034},
  year={2022},
  publisher={Elsevier}
}
```

## Collaboration

If you wish to collaborate with this project please contact me to my email address below.

*Have fun!*

Copyright (c) 2021, Juan Ignacio Deza
email: ignacio.deza@uwe.ac.uk
