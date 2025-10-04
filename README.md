# qNoise: A Generator of Non-Gaussian Colored Noise

<p align="center">
  <a href="https://doi.org/10.1016/j.softx.2022.101034" target="_blank">
    **📰 Paper DOI: 10.1016/j.softx.2022.101034**
  </a>
  &nbsp;&middot;&nbsp;
  <a href="https://ignaciodeza.github.io/qNoise/" target="_blank">
    **💻 Live Demo & Frontend Tool**
  </a>
</p>

***

## Project Overview

**qNoise** is an efficient, stand-alone **C++ library** for generating self-correlated, non-Gaussian colored random noise. It is designed to be a versatile noise source for numerical simulations, experimental control, and statistical analysis across various complex systems.

The noise is defined by only two key parameters, offering precise control over its statistical and correlation properties:

* $\tau$ **(tau):** Controls the **autocorrelation time**.
* $q$ **(q-parameter):** Controls the **statistics** (level of non-Gaussianity).

The generator is based on a stochastic differential equation (SDE) integrated via the Heun method (a second-order Runge-Kutta type scheme).

### Statistical Regimes

| Parameter | Regime | Description |
| :---: | :--- | :--- |
| **$q = 1$** | **Gaussian (Ornstein-Uhlenbeck)** | The noise smoothly approaches a standard colored Gaussian process with autocorrelation $\tau$. |
| **$q < 1$** | **Sub-Gaussian** | The noise is **bounded** (finite variance and compact support). |
| **$q > 1$** | **Supra-Gaussian** | The noise has heavier tails (non-Gaussian statistics). |

***

## Requirements & Installation

The library is entirely **stand-alone** with no dependencies beyond the C++ standard library. It requires **C++11 or higher** due to its use of the `<random>` library.

### Installation & Testing (Short Story)

To quickly clone, compile, and verify the installation:

```bash
# 1. Clone the repository
git clone [https://github.com/ignaciodeza/qNoise.git](https://github.com/ignaciodeza/qNoise.git)
cd qNoise/test

# 2. Compile the qNoise library and the test executable
make test

# 3. Run the battery of tests (verifies functionality and parameters)
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

## License

This project is licensed under the **MIT License**. For the full terms, see the [LICENSE](https://github.com/ignaciodeza/qNoise/blob/main/LICENSE) file in the repository root.

Copyright (c) 2021-2025, J. Ignacio Deza

email: ignacio.deza@uwe.ac.uk
