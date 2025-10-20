# qNoise: C++ Implementation

<p align="center">
  <a href="https://doi.org/10.1016/j.softx.2022.101034" target="_blank">
    <strong>Paper DOI: 10.1016/j.softx.2022.101034</strong>
  </a>
  &nbsp;&middot;&nbsp;
  <a href="https://ignaciodeza.github.io/qNoise/" target="_blank">
    <strong>Live Demo</strong>
  </a>
</p>

***

## Overview

High-performance C++ library for generating non-Gaussian colored noise. This is the original implementation with no external dependencies beyond the C++ standard library.

***

## Requirements & Installation

The library is entirely **stand-alone** with no dependencies. It requires **C++11 or higher** due to its use of the `<random>` library.

### Quick Start

To quickly clone, compile, and verify the installation:

```bash
# 1. Clone the repository
git clone https://github.com/ignaciodeza/qNoise.git
cd qNoise/test

# 2. Compile the qNoise library and the test executable
make test

# 3. Run the battery of tests
./runTest.sh
```

If you see `All tests OK.` at the end of the process, qNoise is installed and working on your system.

### Detailed Installation

Clone the repository:
```bash
git clone https://github.com/ignaciodeza/qNoise.git
```

To compile the test, navigate to the test folder:
```bash
cd qNoise/test
```

Compile the test using the qNoise library:
```bash
make test
```

Run the battery of tests:
```bash
./runTest.sh
```

In less than a second, examples of several types of noise will be shown in the terminal. Files will be saved to disk. You should see several sets of parameters and in the last line: `All tests OK.`

For cleaning the files and executables from the test folder:
```bash
make clean
```

***

## Usage

### Source Code Integration

Simply move both `qNoise.h` and `qNoise.cpp` to your project folder. No other installation is necessary.

Include as `#include "qNoise.h"` in your projects.

In some older systems you may need to add `-std=gnu++11` to your compilation flags.

### Library Methods

The library provides four methods:

**Gaussian white noise generator** (for convenience)
```cpp
double gaussWN();
```

**Gaussian colored noise generator** (Ornstein-Uhlenbeck)
```cpp
double orsUhl(double x, double tau, double H);
```

**qNoise generator**, valid in the whole range, where *tau* and the variance of the noise diverge for *q = 5/3*
```cpp
double qNoise(double x, double tau, double q, double H, double sqrt_H);
```

**Normalized qNoise**, where *tau* and the variance are independent of q, valid sufficiently far away from *q = 5/3*
```cpp
double qNoiseNorm(double x, double tau, double q, double H, double sqrt_H);
```

**Random number generator seeding methods**
```cpp
void seedManual(unsigned Seed);
void seedTimer();
```

The timer is used by default.

### Example Usage

```cpp
#include "qNoise.h"
#include <iostream>
#include <cmath>

int main() {
    qNoiseGen gen;
    gen.seedManual(42);
    
    double x = 0.0;
    double tau = 1.0;
    double q = 1.5;
    double H = 0.01;
    double sqrt_H = std::sqrt(H);
    
    for(int i = 0; i < 1000; i++) {
        x = gen.qNoise(x, tau, q, H, sqrt_H);
        std::cout << x << std::endl;
    }
    
    return 0;
}
```

Compile with:
```bash
g++ -std=c++11 -O3 your_code.cpp qNoise.cpp -o your_program
```

***

## Standalone Executable

The executable `test_qNoise` can be used as a standalone noise generator.

It accepts four arguments:
```bash
./test_qNoise tau H q N
```

- **tau** is the autocorrelation
- **H** is the integration step
- **q** controls the statistics
- **N** is the number of points to calculate

Example:
```bash
./test_qNoise 1 0.01 1.2 1000
```

This generates 1000 points of supra-Gaussian noise (q=1.2) with tau=1 (relative to H=0.01) and saves them to the file:
```
qNoise_1_0.01_1.2_1000.txt
```

The filename explicitly contains all the parameters.

This file together with `runTest.sh` can be edited to better suit your needs and compiled using the same scheme provided above.

***

## Unit Tests

A set of unit tests is provided. It compares the histograms to the theoretical PDFs and calculates the square root of sum of squared differences as a percentage of the maximum height of the distribution.

This provides a metric for the "goodness" of the histogram of the generated noise. Note that it may take time (N) for the histogram to take the shape of the PDF, and this time may be considerably higher for highly correlated noise.

To access the unit test, first uncomment the following line in `qNoise.cpp`:

```cpp
// #define UNIT_TEST
#ifdef UNIT_TEST
#include "unit_tests.cpp"
#endif
```

Then compile and run with a command similar to this (it may vary on your system):

```bash
c++ -std=gnu++11 qNoise.cpp -o qNoise; ./qNoise
```

The limit of 10% difference for passing the test is arbitrary, but provides a good guideline for most applications.

***

## Integration in Larger Projects

### CMake Example

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

add_executable(my_program 
    main.cpp
    qNoise.cpp
)
```

### Makefile Example

```makefile
CXX = g++
CXXFLAGS = -std=c++11 -O3 -Wall

my_program: main.o qNoise.o
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o my_program
```

***

## References

Please read the following [paper](https://doi.org/10.1016/j.softx.2022.101034) which describes the software and its applications.

## Citing

If you use this software please cite us:

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

## License

This project is licensed under the **MIT License**. For the full terms, see the [LICENSE](https://github.com/ignaciodeza/qNoise/blob/main/LICENSE) file in the repository root.

Copyright (c) 2021-2025, J. Ignacio Deza

email: ignacio.deza@uwe.ac.uk