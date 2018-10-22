#ifndef FFT_TEST_H_
#define FFT_TEST_H_

#include <fftw3.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "gnuplot.h"

#include <vector>
#include <iterator>

#define REAL 0
#define IMAG 1

#define MAX_BPM    120
#define F_SAMPLING 44100
#define N_SAMPLES  (F_SAMPLING / (MAX_BPM / 60 * 2))
#define N_BINS     (N_SAMPLES/2)-1

int main() {
  // Sampling frequency in Hz
  double Fs = 200;
  // Frequency delta: frequency distance between two succeding bins in FFT
  double dF = Fs / F_SAMPLING;
  // Sample time, duration in seconds processed in a single FFT pass
  double Ts = 1 / Fs;

  double A = 1.0;
  double freq_a = 440.0;
  double freq_c = std::pow(2.0,(52.0-49.0)/12.0)*freq_a; // 523.251;
  double freq_e = std::pow(2.0,(56.0-49.0)/12.0)*freq_a; // 659.255;
  std::cout << freq_c << ", " << freq_e << std::endl;

  std::vector<double> theta(N_SAMPLES);

  fftw_complex signal[N_SAMPLES];
  fftw_complex result[N_SAMPLES];

  Gnuplot gp_signal, gp_db_spectrum;
  gp_signal.plot_xrange(0, 882);

  for (int i = 0; i < N_SAMPLES; i++) {
    theta[i]        = (static_cast<double>(i)/static_cast<double>(N_SAMPLES))
                    * 2.0 * M_PI;
    signal[i][REAL] = A   * sin(freq_a * theta[i])
                    // + A/2 * sin(freq_c * theta[i])
                    // + A/3 * sin(freq_e * theta[i])
                      ;
    signal[i][IMAG] = 0;

    gp_signal.write_data(i, signal[i][REAL]);
  }

  fftw_plan plan = fftw_plan_dft_1d(N_SAMPLES,
                                    signal, result,
                                    FFTW_FORWARD,
                                    FFTW_ESTIMATE);
  fftw_execute(plan);

  // Post-process frequency spectrum: transform to decibel
  std::vector<double> db_spectrum(N_BINS);
  for(int i = 0; i < N_BINS; i++){
     db_spectrum[i] = (20 *
                        log10(result[i][REAL] * result[i][REAL]
                            + result[i][IMAG] * result[i][IMAG]))
                      );
  }
  std::vector<double>::iterator max = std::max_element(db_spectrum.begin(),
                                                       db_spectrum.end());
  std::cout << "Detected frequency: "
            << std::distance(db_spectrum.begin(), max)
            << " Hz at velocity " << *max << std::endl;

  gp_db_spectrum.plot_xrange(350, N_BINS);

  for(int i = 0; i < N_BINS; i++) {
    // std::cout << db_spectrum[i] << std::endl;
    gp_db_spectrum.write_data(i, db_spectrum[i]);
  }

  fftw_destroy_plan(plan);
  return 0;
}

#endif
