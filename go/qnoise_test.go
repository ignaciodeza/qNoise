package qnoise

import (
	"math"
	"testing"
)

func TestGaussWN(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	value := gen.GaussWN()
	if math.IsNaN(value) || math.IsInf(value, 0) {
		t.Errorf("GaussWN returned invalid value: %v", value)
	}
}

func TestOrsUhl(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	x := 0.0
	tau := 1.0
	h := 0.01

	for i := 0; i < 100; i++ {
		x = gen.OrsUhl(x, tau, h)
		if math.IsNaN(x) || math.IsInf(x, 0) {
			t.Errorf("OrsUhl returned invalid value at iteration %d: %v", i, x)
		}
	}
}

func TestQNoise(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	x := 0.0
	tau := 1.0
	q := 1.5
	h := 0.01
	sqrtH := math.Sqrt(h)

	for i := 0; i < 100; i++ {
		x = gen.QNoise(x, tau, q, h, sqrtH)
		if math.IsNaN(x) || math.IsInf(x, 0) {
			t.Errorf("QNoise returned invalid value at iteration %d: %v", i, x)
		}
	}
}

func TestQNoiseSubGaussian(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	x := 0.0
	tau := 1.0
	q := 0.5 // Sub-Gaussian (bounded)
	h := 0.01
	sqrtH := math.Sqrt(h)

	etaCut := 1 / math.Sqrt(tau*(1-q))

	for i := 0; i < 1000; i++ {
		x = gen.QNoise(x, tau, q, h, sqrtH)
		if math.IsNaN(x) || math.IsInf(x, 0) {
			t.Errorf("QNoise (sub-Gaussian) returned invalid value at iteration %d: %v", i, x)
		}
		// Check boundedness
		if math.Abs(x) > etaCut*1.1 { // Allow 10% margin
			t.Errorf("QNoise (sub-Gaussian) exceeded bounds at iteration %d: %v (cut: %v)", i, x, etaCut)
		}
	}
}

func TestGenerate(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	n := 1000
	noise := gen.Generate(1.0, 1.5, n, 0.01, -1, false)

	if len(noise) != n {
		t.Errorf("Generate returned wrong length: got %d, want %d", len(noise), n)
	}

	for i, v := range noise {
		if math.IsNaN(v) || math.IsInf(v, 0) {
			t.Errorf("Generate returned invalid value at index %d: %v", i, v)
		}
	}
}

func TestGenerateOrnsteinUhlenbeck(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	n := 1000
	noise := gen.GenerateOrnsteinUhlenbeck(1.0, n, 0.01, -1, false)

	if len(noise) != n {
		t.Errorf("GenerateOrnsteinUhlenbeck returned wrong length: got %d, want %d", len(noise), n)
	}

	for i, v := range noise {
		if math.IsNaN(v) || math.IsInf(v, 0) {
			t.Errorf("GenerateOrnsteinUhlenbeck returned invalid value at index %d: %v", i, v)
		}
	}
}

func TestGenerateWhiteNoise(t *testing.T) {
	gen := NewGeneratorWithSeed(42)
	n := 1000
	noise := gen.GenerateOrnsteinUhlenbeck(0, n, 0.01, -1, true)

	if len(noise) != n {
		t.Errorf("GenerateOrnsteinUhlenbeck (white) returned wrong length: got %d, want %d", len(noise), n)
	}

	for i, v := range noise {
		if math.IsNaN(v) || math.IsInf(v, 0) {
			t.Errorf("GenerateOrnsteinUhlenbeck (white) returned invalid value at index %d: %v", i, v)
		}
	}
}

func TestPackageLevelFunctions(t *testing.T) {
	SetSeed(42)

	n := 100
	noise := Generate(1.0, 1.5, n, 0.01, -1, false)

	if len(noise) != n {
		t.Errorf("Package-level Generate returned wrong length: got %d, want %d", len(noise), n)
	}

	gaussNoise := GenerateOrnsteinUhlenbeck(1.0, n, 0.01, -1, false)

	if len(gaussNoise) != n {
		t.Errorf("Package-level GenerateOrnsteinUhlenbeck returned wrong length: got %d, want %d", len(gaussNoise), n)
	}
}

func TestReproducibility(t *testing.T) {
	// Test that same seed produces same results
	gen1 := NewGeneratorWithSeed(42)
	gen2 := NewGeneratorWithSeed(42)

	noise1 := gen1.Generate(1.0, 1.5, 100, 0.01, -1, false)
	noise2 := gen2.Generate(1.0, 1.5, 100, 0.01, -1, false)

	for i := range noise1 {
		if noise1[i] != noise2[i] {
			t.Errorf("Reproducibility failed at index %d: %v != %v", i, noise1[i], noise2[i])
		}
	}
}

func BenchmarkQNoise(b *testing.B) {
	gen := NewGeneratorWithSeed(42)
	x := 0.0
	sqrtH := math.Sqrt(0.01)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		x = gen.QNoise(x, 1.0, 1.5, 0.01, sqrtH)
	}
}

func BenchmarkGenerate(b *testing.B) {
	gen := NewGeneratorWithSeed(42)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		_ = gen.Generate(1.0, 1.5, 1000, 0.01, -1, false)
	}
}