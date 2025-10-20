/*
qNoise: A generator of non-Gaussian colored noise
Copyright (c) 2021-2025 J. Ignacio Deza

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

qNoise - Non-Gaussian colored noise generator
Contact: ignacio.deza@uwe.ac.uk
Paper: https://doi.org/10.1016/j.softx.2022.101034
*/

package qnoise

import (
	"math"
	"math/rand"
	"time"
)

// Generator holds the random number generator state
type Generator struct {
	rng *rand.Rand
}

// NewGenerator creates a new qNoise generator with a time-based seed
func NewGenerator() *Generator {
	return &Generator{
		rng: rand.New(rand.NewSource(time.Now().UnixNano())),
	}
}

// NewGeneratorWithSeed creates a new qNoise generator with a specific seed
func NewGeneratorWithSeed(seed int64) *Generator {
	return &Generator{
		rng: rand.New(rand.NewSource(seed)),
	}
}

// GaussWN generates a single Gaussian white noise value
func (g *Generator) GaussWN() float64 {
	return g.rng.NormFloat64()
}

// OrsUhl generates a single Ornstein-Uhlenbeck noise step
// Parameters:
//   - eta: current noise value
//   - tau: autocorrelation time
//   - h: integration time step
func (g *Generator) OrsUhl(eta, tau, h float64) float64 {
	expFactor := math.Exp(-h / tau)
	return eta*expFactor + math.Sqrt((1-math.Exp(-2*h/tau))/(2*tau))*g.rng.NormFloat64()
}

// potQNoisePrime calculates the derivative of the qNoise potential (private)
func potQNoisePrime(eta, tau, q float64) float64 {
	return (eta / (1 + (eta*eta*tau*(q-1)))) / tau
}

// QNoise generates a single qNoise step using Heun's method
// Parameters:
//   - eta: current noise value
//   - tau: autocorrelation time
//   - q: statistics parameter (q=1: Gaussian, q<1: bounded, q>1: heavy-tailed)
//   - h: integration time step
//   - sqrtH: square root of h (pass -1 to compute automatically)
func (g *Generator) QNoise(eta, tau, q, h, sqrtH float64) float64 {
	if sqrtH < 0 {
		sqrtH = math.Sqrt(h)
	}

	etaCut := 1 / math.Sqrt(tau*(1-q))
	countError := 0

	for {
		kHeun := h * potQNoisePrime(eta, tau, q)
		lHeun := sqrtH * g.rng.NormFloat64() / tau
		differential := -h/2*(potQNoisePrime(eta, tau, q)+potQNoisePrime(eta+kHeun+lHeun, tau, q)) + lHeun

		if math.Abs(eta+differential) > etaCut || math.IsNaN(eta+differential) {
			countError++
			if countError > 20 {
				sign := 1.0
				if eta < 0 {
					sign = -1.0
				}
				return sign * etaCut * (0.9 + 0.1*g.rng.Float64())
			}
			if countError > 10 {
				eta = etaCut * g.OrsUhl(eta, tau, h)
			}
			// continue loop
		} else {
			return eta + differential
		}
	}
}

// QNoiseNorm generates normalized qNoise where tau and variance are independent of q
func (g *Generator) QNoiseNorm(eta, tau, q, h, sqrtH float64) float64 {
	adjustedTau := tau * (5 - 3*q) / 2
	return g.QNoise(eta, adjustedTau, q, h, sqrtH)
}

// Generate generates an array of qNoise values
// Parameters:
//   - tau: autocorrelation time
//   - q: statistics parameter
//   - n: number of samples
//   - h: integration time step (use 0.01*tau as default)
//   - tempN: number of transient samples to skip (-1 for auto: 2*tau/h)
//   - norm: use normalized version
func (g *Generator) Generate(tau, q float64, n int, h float64, tempN int, norm bool) []float64 {
	if tau < 0 {
		tau = -tau
	}
	if tau == 0 {
		tau = h
	}

	if tempN < 0 {
		tempN = int(2 * tau / h)
	}

	sqrtH := math.Sqrt(h)
	x := g.rng.NormFloat64() / 100.0
	result := make([]float64, n)

	// Skip transient
	for i := -tempN; i < 0; i++ {
		if norm {
			x = g.QNoiseNorm(x, tau, q, h, sqrtH)
		} else {
			x = g.QNoise(x, tau, q, h, sqrtH)
		}
	}

	// Generate output
	for i := 0; i < n; i++ {
		if norm {
			x = g.QNoiseNorm(x, tau, q, h, sqrtH)
		} else {
			x = g.QNoise(x, tau, q, h, sqrtH)
		}
		result[i] = x
	}

	return result
}

// GenerateOrnsteinUhlenbeck generates an array of Ornstein-Uhlenbeck noise
func (g *Generator) GenerateOrnsteinUhlenbeck(tau float64, n int, h float64, tempN int, whiteNoise bool) []float64 {
	if tau < 0 {
		tau = -tau
	}
	if tau == 0 || whiteNoise {
		// Generate white noise
		result := make([]float64, n)
		for i := 0; i < n; i++ {
			result[i] = g.GaussWN()
		}
		return result
	}

	if tempN < 0 {
		tempN = int(2 * tau / h)
	}

	x := g.rng.NormFloat64() / 100.0
	result := make([]float64, n)

	// Skip transient
	for i := -tempN; i < 0; i++ {
		x = g.OrsUhl(x, tau, h)
	}

	// Generate output
	for i := 0; i < n; i++ {
		x = g.OrsUhl(x, tau, h)
		result[i] = x
	}

	return result
}

// Package-level convenience functions using a default generator

var defaultGenerator = NewGenerator()

// Generate generates qNoise using the default generator
func Generate(tau, q float64, n int, h float64, tempN int, norm bool) []float64 {
	return defaultGenerator.Generate(tau, q, n, h, tempN, norm)
}

// GenerateOrnsteinUhlenbeck generates Ornstein-Uhlenbeck noise using the default generator
func GenerateOrnsteinUhlenbeck(tau float64, n int, h float64, tempN int, whiteNoise bool) []float64 {
	return defaultGenerator.GenerateOrnsteinUhlenbeck(tau, n, h, tempN, whiteNoise)
}

// SetSeed sets the seed for the default generator
func SetSeed(seed int64) {
	defaultGenerator = NewGeneratorWithSeed(seed)
}