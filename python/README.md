# qNoise: Python Package

<p align="center">
  <a href="https://pypi.org/project/qnoise/" target="_blank">
    <strong>PyPI Package</strong>
  </a>
  &nbsp;&middot;&nbsp;
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

Python implementation of qNoise for generating non-Gaussian colored noise. Built with C++ backend via pybind11 for performance, with a Pythonic interface for ease of use.

***

## Installation

```bash
pip install qnoise
```

### System Requirements

**Compiler Required:** qNoise includes C++ extensions that are compiled during installation.

**macOS:**
```bash
xcode-select --install
```

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get install build-essential python3-dev
```

**Windows:**
Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with C++ support.

### Installation Issues

If installation fails, try installing build dependencies first:
```bash
pip install --upgrade pip setuptools wheel pybind11
pip install qnoise
```

***

## Quick Start

```python
import qnoise
import matplotlib.pyplot as plt

# Seed for reproducibility
qnoise.seed_manual(42)

# Generate non-Gaussian colored noise
noise = qnoise.generate(tau=1.0, q=1.5, N=10000)

# Generate Gaussian colored noise (Ornstein-Uhlenbeck)
gauss_noise = qnoise.ornstein_uhlenbeck(tau=1.0, N=10000)

# Plot
plt.plot(noise[:1000])
plt.title('Non-Gaussian Colored Noise (q=1.5)')
plt.xlabel('Time')
plt.ylabel('Value')
plt.show()
```

***

## Parameters

### tau (Autocorrelation Time)
Controls the temporal correlation of the noise:
- `tau = 0`: White noise (uncorrelated)
- `tau > 0`: Colored noise with correlation time tau

### q (Statistics Parameter)
Controls the shape of the statistical distribution:
- `q = 1`: Gaussian (Ornstein-Uhlenbeck process)
- `q < 1`: Sub-Gaussian (bounded support)
- `q > 1`: Supra-Gaussian (heavy tails)

### Additional Parameters
- `N`: Number of samples to generate
- `H`: Integration time step (default: 0.01)
- `temp_N`: Transient samples to discard (default: auto-computed as 2*tau/H)
- `norm`: Use normalized version where tau and variance are independent of q

***

## Functions

### High-Level Interface

**Generate qNoise**
```python
qnoise.generate(tau, q, N=1000, H=0.01, temp_N=-1, norm=False)
```

**Generate Ornstein-Uhlenbeck noise**
```python
qnoise.ornstein_uhlenbeck(tau, N=1000, H=0.01, temp_N=-1, white_noise=False)
```

### Low-Level Interface

For custom integration loops:
```python
qnoise.qnoise_step(x, tau, q, H, sqrt_H)
qnoise.qnoise_norm_step(x, tau, q, H, sqrt_H)
qnoise.ornstein_uhlenbeck_step(x, tau, H)
qnoise.gauss_white_noise()
```

### Seeding
```python
qnoise.seed_manual(seed)  # Set specific seed for reproducibility
qnoise.seed_timer()       # Seed from system time
```

***

## Examples

### Comparing Different Statistical Regimes

```python
import qnoise
import matplotlib.pyplot as plt

qnoise.seed_manual(42)

fig, axes = plt.subplots(3, 2, figsize=(12, 10))

for i, q in enumerate([0.5, 1.0, 1.5]):
    # Generate noise
    noise = qnoise.generate(tau=1.0, q=q, N=10000)
    
    # Time series
    axes[i, 0].plot(noise[:1000])
    axes[i, 0].set_title(f'q={q} Time Series')
    axes[i, 0].set_xlabel('Time')
    axes[i, 0].set_ylabel('Value')
    
    # Distribution
    axes[i, 1].hist(noise, bins=50, density=True, alpha=0.7)
    axes[i, 1].set_title(f'q={q} Distribution')
    axes[i, 1].set_xlabel('Value')
    axes[i, 1].set_ylabel('Probability Density')

plt.tight_layout()
plt.show()
```

### Autocorrelation Analysis

```python
import qnoise
import numpy as np
import matplotlib.pyplot as plt

qnoise.seed_manual(42)

# Generate noise with different correlation times
for tau in [0.1, 1.0, 10.0]:
    noise = qnoise.generate(tau=tau, q=1.0, N=50000)
    
    # Compute autocorrelation
    mean = np.mean(noise)
    noise_centered = noise - mean
    autocorr = np.correlate(noise_centered, noise_centered, mode='full')
    autocorr = autocorr[len(autocorr)//2:]
    autocorr /= autocorr[0]
    
    plt.plot(autocorr[:200], label=f'tau={tau}')

plt.xlabel('Lag')
plt.ylabel('Autocorrelation')
plt.legend()
plt.title('Autocorrelation for Different τ Values')
plt.grid(True, alpha=0.3)
plt.show()
```

### Monte Carlo Simulation with Non-Gaussian Noise

```python
import qnoise
import numpy as np
import matplotlib.pyplot as plt

qnoise.seed_manual(42)

# Simulate a stochastic process: dx/dt = -x + noise
dt = 0.01
N = 10000
noise = qnoise.generate(tau=0.5, q=1.8, N=N, H=dt)

x = np.zeros(N)
for i in range(1, N):
    x[i] = x[i-1] + dt * (-x[i-1] + noise[i])

plt.plot(x)
plt.title('Stochastic Process with Heavy-Tailed Noise (q=1.8)')
plt.xlabel('Time Step')
plt.ylabel('x(t)')
plt.show()
```

***

## Applications

**Numerical Simulations**  
Generate realistic stochastic processes for Monte Carlo simulations in finance, physics, and engineering.

**Algorithm Testing**  
Test the robustness of machine learning models and signal processing algorithms against non-Gaussian noise conditions.

**Data Augmentation**  
Augment training datasets with correlated noise that matches real-world statistical properties.

**Risk Analysis**  
Model extreme events and tail behavior for stress testing and risk assessment in financial systems.

***

## Troubleshooting

### Installation Errors

**"Microsoft Visual C++ 14.0 or greater is required" (Windows)**  
Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with C++ support.

**"command 'gcc' failed with exit code 1" (Linux)**
```bash
sudo apt-get install build-essential python3-dev
```

**"clang: error" (macOS)**
```bash
xcode-select --install
```

### Import Errors

**ModuleNotFoundError: No module named 'qnoise._qnoise'**  
The C++ extension did not compile properly. Try:
```bash
pip install --force-reinstall qnoise
```

### Runtime Issues

**Values seem incorrect or unexpected**
- Verify that tau and q values are reasonable for your application
- Ensure H << tau (typically H = 0.01*tau or smaller)
- For q < 1, noise is bounded by design
- Skip the transient period (first ~2*tau/H points) if starting conditions matter

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