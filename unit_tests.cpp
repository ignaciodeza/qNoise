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

---

Description:
qNoise is a non-Gaussian colored random noise generator. It is a handy source 
of self-correlated noise for a great variety of applications. It depends on 
two parameters only: tau for controlling the autocorrelation, and q for 
controlling the statistics.

- q = 1: Ornstein-Uhlenbeck (colored Gaussian) noise
- q < 1: Bounded noise (sub-Gaussian)
- q > 1: Heavy-tailed noise (supra-Gaussian)

The noise is generated via a stochastic differential equation using the Heun 
method (a second-order Runge-Kutta integration scheme).

Requirements:
- C++11 or higher (uses <random> library)
- No external dependencies

Contact: ignacio.deza@uwe.ac.uk

Citation:
Deza, J.I., Ihshaish, H. (2022). qNoise: A generator of non-Gaussian colored 
noise. SoftwareX, 18, 101034. https://doi.org/10.1016/j.softx.2022.101034
*/


const int bins = 50;
double histComparison(std::vector<double> &value,
                      std::vector<double> &value_theory);
void calculatePDF(std::vector<double> &key, std::vector<double> &value_theory,
                  double tau, double q);
void genHistogram(const std::vector<double> &data, std::vector<double> &key,
                  std::vector<double> &value, long int N, int bins);
double test_gaussWN(const std::vector<double> &data, long int N);
double test_orsUhl(const std::vector<double> &data, double tau, double H,
                   long int N);
double test_qNoise(const std::vector<double> &data, double tau, double q,
                   double H, long int N, bool show);
double test_qNoiseNorm(const std::vector<double> &data, double tau, double q,
                       double H, long int N, bool show);

double histComparison(std::vector<double> &value,
                      std::vector<double> &value_theory) {
  double min = *std::min_element(value.begin(), value.end());
  double max = *std::max_element(value.begin(), value.end());
  double distance = 0;
  for (int i = 0; i < value.size(); i++) {
    distance +=
        sqrt((value[i] - value_theory[i]) * (value[i] - value_theory[i]));
    assert(!isnan(distance));
  }
  // Average of distance as a percentage of total size of the histogram

  int dummy = value.size();
  std::vector<double>().swap(value);
  std::vector<double>().swap(value_theory);
  return distance / dummy / (max - min) * 100;
}

/*
 * Generate a histogram from the data with a fixed number of bins the max and
 * min of the data are calculated.
 */
void genHistogram(const std::vector<double> &data, std::vector<double> &key,
                  std::vector<double> &value, long int N, int bins) {
  double min = *std::min_element(data.begin(), data.end());
  double max = *std::max_element(data.begin(), data.end());

  for (double x = min; x < max; x += (max - min) / bins) {
    key.push_back(x);
    value.push_back(0);
  }

  for (int i = 0; i < N; i++) {
    int x = int(round((data[i] - min) / (max - min) * bins));
    assert(isnormal(x + 1));
    value[x] += 1.0 / N / ((max - min) * bins);
  }

  return;
}

void calculatePDF(std::vector<double> &key, std::vector<double> &value_theory,
                  double tau, double q) {
  double total = 0;
  double min = *std::min_element(key.begin(), key.end());
  double max = *std::max_element(key.begin(), key.end());

  if (q == 1) {
    for (int i = 0; i < key.size(); i++) {
      double v = exp(-tau * key[i] * key[i]);
      assert(!isnan(v));
      value_theory.push_back(v);
      total += v;
    }
  } else if (q < 1) {
    double w = pow((1 - q) * tau, -.5);
    // cutoff
    for (int i = 0; i < key.size(); i++) {
      double v = pow((1 - (key[i] / w) * (key[i] / w)), (1 / (1 - q)));
      if (isnan(v))
        // Some bins fall outside the defined support
        v = 0;
      value_theory.push_back(v);
      total += v;
    }
  } else if (q > 1) {
    for (int i = 0; i < key.size(); i++) {
      double v = pow((1 + tau * (q - 1) * key[i] * key[i]), (1 / (1 - q)));
      assert(!isnan(v));
      value_theory.push_back(v);
      total += v;
    }
  }
  for (int i = 0; i < key.size(); i++) {
    value_theory[i] /= total * ((max - min) * bins);
  }
  return;
}

/*
 * Unit Test for gaussWN(). get a distribution and compare it with the
 * theoretical PDF.
 */
double test_gaussWN(const std::vector<double> &data, long int N) {
  std::vector<double> key, value, value_theory;

  // Generate histogram from the data
  genHistogram(data, key, value, N, bins);

  // Theoretical value from formula
  calculatePDF(key, value_theory, 0.5, 1);

  // Calculate the "distance" between the histogram and the PDF
  std::vector<double>().swap(key);
  return histComparison(value, value_theory);
}

/*
 * Unit Test for orsUhl().get distributions with different tau and compare
 * them to the theoretical PDF.
 */
double test_orsUhl(const std::vector<double> &data, double tau, double H,
                   long int N) {
  std::vector<double> key, value, value_theory;

  // Generate histogram from the data
  genHistogram(data, key, value, N, bins);
  // Theoretical value from formula
  calculatePDF(key, value_theory, tau, 1);
  // Calculate the "distance" between the histogram and the PDF

  std::vector<double>().swap(key);
  return histComparison(value, value_theory);
}

/*
 * Unit Test for qNoise(). Will generate Distributions with different tau and
 * q and compare them to the theoretical PDF.
 */
double test_qNoise(const std::vector<double> &data, double tau, double q,
                   double H, long int N, bool show = false) {
  std::vector<double> key, value, value_theory;

  // Generate histogram from the data
  genHistogram(data, key, value, N, bins);
  // Theoretical value from formula
  calculatePDF(key, value_theory, tau, q);
  // Calculate the "distance" between the histogram and the PDF
  std::vector<double>().swap(key);
  return histComparison(value, value_theory);
}

double test_qNoiseNorm(const std::vector<double> &data, double tau, double q,
                       double H, long int N, bool show = false) {
  tau = tau * (5 - 3 * q) / 2;
  return test_qNoise(data, tau, q, H, N, show);
}

/* Main function for unit testing purposes only */
int main() {
  long int NArray[] = {int(1e4),int(1e5),int(1e6),int(1e7)};
  double limit = 10;
  double q[] = {0.1 , 0.2 , 0.3 , 0.4 , 0.5 , 0.6 , 0.7 , 0.8 , 0.9 , 1.  , 1.1 , 1.2 , 1.3 };
  double tau[] = {0.1 , 1., 10., 100.};
  double H = 0.001, sqrt_H = sqrt(H);
  std::vector<double> data(int(1e7));


  qNoiseGen gen;
  // gen.seedManual(01234567);  //Manual seeding, comment for automatic seeding
  bool simple_example = false;

  if (simple_example) {
    double t = 100, qi = .001;
    int N = int(1e8);
    double old = gen.gaussWN();
    // First point
    for (int i = 0; i < N; i++) {
      double tmp = gen.qNoiseNorm(old, t, qi, H, sqrt_H);
      assert(isnormal(tmp));
      data[i] = tmp;
      old = tmp;
    }

    double dist = test_qNoiseNorm(data, t, qi, H, N, true);
    std::string test =
        (dist <= 5) ? "..................PASSED" : "..................FAILED";
    std::cout << "tau: " << t << ", q: " << qi << ", N: " << N
              << ", distance: " << dist << " \%" << test << "\n";
  }
  std::cout << "Unit test of Algorithms by the square of the difference "
               "between histogram and theoretical PDF.\n";


  // White Noise
  for (long int N : NArray) {
    for (int i = 0; i < N; i++)
      data[i] = gen.gaussWN();

    double x = test_gaussWN(data, N);
    std::string test =
        (x <= limit) ? "PASSED" : "FAILED";
    std::cout << "Gaussian White Noise, N= " << N << ", difference= " << x
              << "\%, limit= " << limit << "\%, result:.................." << test << std::endl;
  }

  // Ornstein - Uhlenbeck noise
  for (double t : tau) {
    for (long int N : NArray) {

      double old = gen.gaussWN();
      // First point
      for (int i = 0; i < N; i++) {
        double tmp = gen.orsUhl(old, t, H);
        assert(isnormal(tmp));
        data[i] = tmp;
        old = tmp;
      }

      double x = test_orsUhl(data, t, H, N);
      std::string test = (x <= limit) ? "PASSED" : "FAILED";
      std::cout << "Ornstein-Uhlenbeck, N= " << N << ", tau= " << t
                << ", difference= " << x << "\%, limit= " << limit
                << "\%, result:.................." << test << std::endl;
    }
  }

  // qNoise
for (long int N : NArray) 
  for (double t : tau)
    for (double qi : q){

        double old = gen.gaussWN();
        // First point
        for (int i = 0; i < N; i++) {
          double tmp = gen.qNoise(old, t, qi, H, sqrt_H);
          assert(isnormal(tmp));
          data[i] = tmp;
          old = tmp;
        }

        double x = test_qNoise(data, t, qi, H, N);
        std::string test = (x <= limit) ? "PASSED"
                                        : "FAILED";
        std::cout << "Qnoise, N= " << N << ", tau= " << t << ",q= " << qi
                  << ", difference= " << x << "\%, limit= " << limit
                  << "\%, result:.................." << test << std::endl;
                  
      }

  // Normalized qNoise
  for (double t : tau)
    for (double qi : q)
      for (long int N : NArray) {

        double old = gen.gaussWN();
        // First point
        for (int i = 0; i < N; i++) {
          double tmp = gen.qNoiseNorm(old, t, qi, H, sqrt_H);
          assert(isnormal(tmp));
          data[i] = tmp;
          old = tmp;
        }

        double x = test_qNoise(data, t, qi, H, N);
        std::string test = (x <= limit) ? "PASSED"
                                        : "FAILED";
        std::cout << "Normalized Qnoise, N= " << N << ", tau= " << t << ",q= " << qi
                  << ", difference= " << x << "\%, limit= " << limit
                  << "\%, result:.................." << test << std::endl;
      }

  return 0;
}
