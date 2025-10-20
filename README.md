# qNoise: A Generator of Non-Gaussian Colored Noise

<p align="center">
  <a href="https://doi.org/10.1016/j.softx.2022.101034" target="_blank">
    <strong>Paper DOI: 10.1016/j.softx.2022.101034</strong>
  </a>
  &nbsp;&middot;&nbsp;
  <a href="https://ignaciodeza.github.io/qNoise/" target="_blank">
    <strong>Live Demo & Frontend Tool</strong>
  </a>
  &nbsp;&middot;&nbsp;
  <a href="https://pypi.org/project/qnoise/" target="_blank">
    <strong>PyPI Package</strong>
  </a>
</p>

***

## Project Overview

**qNoise** is an efficient library for generating self-correlated, non-Gaussian colored random noise. Available in **C++**, **Python**, and **Go**, it is designed as a versatile noise source for numerical simulations, experimental control, and statistical analysis across various complex systems.

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

## Available Implementations

### Python

The recommended implementation for most users. Available via PyPI with C++ backend for performance.

```bash
pip install qnoise
```

```python
import qnoise

# Generate non-Gaussian colored noise
noise = qnoise.generate(tau=1.0, q=1.5, N=10000)

# Generate Gaussian colored noise (Ornstein-Uhlenbeck)
gauss_noise = qnoise.ornstein_uhlenbeck(tau=1.0, N=10000)
```

**[→ Python Documentation](python/README.md)**

### C++

High-performance standalone implementation with no dependencies beyond the C++ standard library.

```bash
git clone https://github.com/ignaciodeza/qNoise.git
cd qNoise/cpp
```

```cpp
#include "qNoise.h"

qNoiseGen gen;
double x = 0.0;
for(int i = 0; i < 1000; i++) {
    x = gen.qNoise(x, tau, q, H, sqrt_H);
}
```

**[→ C++ Documentation](cpp/README.md)**

### Go

Lightweight implementation suitable for cloud deployment and microservices.

```bash
go get github.com/ignaciodeza/qNoise/go
```

**[→ Go Documentation](go/README.md)**

***

## Applications

**Monte Carlo Simulations**  
Generate realistic non-Gaussian stochastic processes for financial risk models, reliability analysis, and complex system simulations.

**Algorithm Robustness Testing**  
Test machine learning models and signal processing algorithms against realistic heavy-tailed and correlated noise conditions.

**Rare Event Simulation**  
Model extreme events and tail behavior using supra-Gaussian noise for stress testing and risk assessment.

**Experimental Control**  
Provide synthetic noise sources for laboratory experiments and control systems requiring specific statistical properties.

***

## Repository Structure

```
qNoise/
├── cpp/              # C++ implementation and headers
├── python/           # Python package with pybind11 bindings
├── go/               # Go implementation
├── test/             # C++ test suite and validation
└── docs/             # Documentation and interactive demo
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

## Collaboration

If you wish to collaborate with this project please contact me at the email address below.

## License

This project is licensed under the **MIT License**. For the full terms, see the [LICENSE](https://github.com/ignaciodeza/qNoise/blob/main/LICENSE) file in the repository root.

Copyright (c) 2021-2025, J. Ignacio Deza

email: ignacio.deza@uwe.ac.uk