# qNoise: A Generator of non-Gaussian Colored Noise

## Description

**qNoise** is a non-gaussian colored random noise generator. It is a handy source of *self-correlated* noise for a great variety of applications. It depends on two parameters only: **tau** for controlling the autocorrelation, and **q** for controlling the statistics. This noise tends smoothly  for *q = 1* to an  *Ornstein-Uhlenbeck* (colored gaussian) noise with autocorrelation *tau*. for *q < 1* it is **bounded noise** and it is **supra-Gaussian** for *q > 1*.

The noise is generated  via a stochastic differential equation using the Heun method (a second order Runge-Kutta type integration scheme) and it is implemented as a stand-alone library in c++.

It Useful as input for numerical simulations, as a source of noise for controlling experiments using synthetic noise via micro-controllers and for a wide variety of applications.

## Requirements

It is a stand-alone library with no dependencies other than the standard libraries.
Due to it's use of some functions from the **\<random\>** library the library currently works on *c++11 or higher* **only**.
This should be OK for most Macs and new Linux systems.
In some older systems it is possible that you need to add `-std=gnu++11` to your compilation flags.

## Installation

####Short story:
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
at the end of the process then **qNoise** is installed and works in your system, if you don't want to know more, you can go directly to **Usage**.

####Long story:

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

####Source Code
Just move both `qNoise.h` and `qNoise.cpp` to the folder where your code is. No other installation is necessary.
Include as `#include<qNoise.h>`in your projects.

In some older systems is possible that you need to add `-std=gnu++11` to your compilation flags.

####Executable
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

This file can be edited to better suit your needs and compiled using the same scheme provided above.

##References

Please read the preprint included in the repository for further information.
This manuscript is being prepared for its submission to a Scientific Journal

##Collaboration

If you wish to collaborate with this project please contact me to my email address below.

*Have fun!*

Copyright (c) 2015, Juan Ignacio Deza
email: juan.ignacio.deza@upc.edu
