# qNoise: Go Implementation

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

Lightweight Go implementation of qNoise for generating non-Gaussian colored noise. Suitable for cloud deployments, microservices, and high-concurrency applications.

***

## Installation

```bash
go get github.com/ignaciodeza/qNoise/go
```

***

## Quick Start

```go
package main

import (
    "fmt"
    "github.com/ignaciodeza/qNoise/go"
)

func main() {
    // Generate non-Gaussian colored noise
    noise := qnoise.Generate(1.0, 1.5, 1000, 0.01, -1, false)
    
    // Print first 10 values
    for i := 0; i < 10; i++ {
        fmt.Printf("%.6f\n", noise[i])
    }
}
```

***

## Usage

### Simple Interface

For most use cases, use the package-level functions:

```go
import "github.com/ignaciodeza/qNoise/go"

// Generate qNoise
noise := qnoise.Generate(tau, q, n, h, tempN, norm)

// Generate Ornstein-Uhlenbeck (Gaussian colored noise)
gaussNoise := qnoise.GenerateOrnsteinUhlenbeck(tau, n, h, tempN, false)

// Set seed for reproducibility
qnoise.SetSeed(42)
```

### Advanced Interface

For multiple independent generators or custom seeding:

```go
import "github.com/ignaciodeza/qNoise/go"

// Create generator with specific seed
gen := qnoise.NewGeneratorWithSeed(42)

// Generate noise
noise := gen.Generate(1.0, 1.5, 1000, 0.01, -1, false)

// Use low-level functions for custom integration
var x float64 = 0.0
sqrtH := math.Sqrt(0.01)
for i := 0; i < 1000; i++ {
    x = gen.QNoise(x, 1.0, 1.5, 0.01, sqrtH)
    fmt.Println(x)
}
```

***

## API Reference

### Package Functions

**Generate(tau, q float64, n int, h float64, tempN int, norm bool) []float64**  
Generate array of qNoise values using the default generator.

Parameters:
- `tau`: Autocorrelation time
- `q`: Statistics parameter (q=1: Gaussian, q<1: bounded, q>1: heavy-tailed)
- `n`: Number of samples
- `h`: Integration time step
- `tempN`: Transient samples to skip (-1 for auto: 2*tau/h)
- `norm`: Use normalized version

**GenerateOrnsteinUhlenbeck(tau float64, n int, h float64, tempN int, whiteNoise bool) []float64**  
Generate Ornstein-Uhlenbeck (Gaussian colored) noise.

**SetSeed(seed int64)**  
Set seed for the default generator.

### Generator Type

**NewGenerator() *Generator**  
Create generator with time-based seed.

**NewGeneratorWithSeed(seed int64) *Generator**  
Create generator with specific seed.

#### Generator Methods

**GaussWN() float64**  
Generate single Gaussian white noise value.

**OrsUhl(eta, tau, h float64) float64**  
Generate single Ornstein-Uhlenbeck step.

**QNoise(eta, tau, q, h, sqrtH float64) float64**  
Generate single qNoise step.

**QNoiseNorm(eta, tau, q, h, sqrtH float64) float64**  
Generate single normalized qNoise step.

**Generate(tau, q float64, n int, h float64, tempN int, norm bool) []float64**  
Generate array of qNoise values.

**GenerateOrnsteinUhlenbeck(tau float64, n int, h float64, tempN int, whiteNoise bool) []float64**  
Generate array of Ornstein-Uhlenbeck values.

***

## Examples

### Basic Usage

```go
package main

import (
    "fmt"
    qnoise "github.com/ignaciodeza/qNoise/go"
)

func main() {
    // Set seed for reproducibility
    qnoise.SetSeed(42)
    
    // Generate heavy-tailed noise
    noise := qnoise.Generate(1.0, 1.8, 10000, 0.01, -1, false)
    
    fmt.Printf("Generated %d samples\n", len(noise))
    fmt.Printf("First value: %.6f\n", noise[0])
}
```

### Multiple Generators

```go
package main

import (
    "fmt"
    qnoise "github.com/ignaciodeza/qNoise/go"
)

func main() {
    // Create two independent generators
    gen1 := qnoise.NewGeneratorWithSeed(42)
    gen2 := qnoise.NewGeneratorWithSeed(123)
    
    // Generate different noise sequences
    noise1 := gen1.Generate(1.0, 1.5, 1000, 0.01, -1, false)
    noise2 := gen2.Generate(1.0, 1.5, 1000, 0.01, -1, false)
    
    fmt.Printf("Gen1 first: %.6f\n", noise1[0])
    fmt.Printf("Gen2 first: %.6f\n", noise2[0])
}
```

### Custom Integration Loop

```go
package main

import (
    "fmt"
    "math"
    qnoise "github.com/ignaciodeza/qNoise/go"
)

func main() {
    gen := qnoise.NewGeneratorWithSeed(42)
    
    var x float64 = 0.0
    tau := 1.0
    q := 1.5
    h := 0.01
    sqrtH := math.Sqrt(h)
    
    // Skip transient
    transient := int(2 * tau / h)
    for i := 0; i < transient; i++ {
        x = gen.QNoise(x, tau, q, h, sqrtH)
    }
    
    // Generate and process
    for i := 0; i < 1000; i++ {
        x = gen.QNoise(x, tau, q, h, sqrtH)
        fmt.Printf("%.6f\n", x)
    }
}
```

### Concurrent Generation

```go
package main

import (
    "fmt"
    "sync"
    qnoise "github.com/ignaciodeza/qNoise/go"
)

func main() {
    var wg sync.WaitGroup
    numWorkers := 4
    
    for i := 0; i < numWorkers; i++ {
        wg.Add(1)
        go func(id int) {
            defer wg.Done()
            
            // Each goroutine needs its own generator
            gen := qnoise.NewGeneratorWithSeed(int64(id))
            noise := gen.Generate(1.0, 1.5, 1000, 0.01, -1, false)
            
            fmt.Printf("Worker %d generated %d samples\n", id, len(noise))
        }(i)
    }
    
    wg.Wait()
}
```

***

## Parameters

### tau (Autocorrelation Time)
Controls the temporal correlation:
- `tau = 0`: White noise
- `tau > 0`: Colored noise with correlation time tau

### q (Statistics Parameter)
Controls the distribution:
- `q = 1`: Gaussian (Ornstein-Uhlenbeck)
- `q < 1`: Sub-Gaussian (bounded)
- `q > 1`: Supra-Gaussian (heavy tails)

### h (Integration Step)
Smaller h gives more accurate results but slower computation. Typically `h = 0.01 * tau`.

### tempN (Transient Samples)
Number of initial samples to discard. Use -1 for automatic (2*tau/h).

***

## Testing

```bash
go test -v
```

***

## Performance

The Go implementation is designed for:
- High concurrency (goroutine-safe when using separate generators)
- Low memory footprint
- Cloud deployment (stateless design)
- Microservices integration

***

## Differences from C++/Python

- **Concurrency**: Each goroutine should use its own `Generator` instance
- **No global state**: No default random seed behaviour like C++ version
- **Functional style**: Emphasizes array generation over step-by-step integration
- **Explicit error handling**: Go idioms for error handling (though current implementation has no errors)

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