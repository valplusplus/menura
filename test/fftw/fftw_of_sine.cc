#ifndef FFT_OF_SINE_CC_
#define FFT_OF_SINE_CC_

#include "fftw_of_sine.h"

  void Sinusoid::generate() {
    std::vector<double> theta(N_SAMPLES);

    for (int i = 0; i < N_SAMPLES; i++) {
      theta[i]        = (static_cast<double>(i)/static_cast<double>(N_SAMPLES))
                      * 2.0 * M_PI;
      signal[i][REAL] = _A  * sin(_freq * theta[i]);
      signal[i][IMAG] = 0;
    }
  }

  void Sinusoid::shift_note_by_halftone(int step) {
    _freq = std::pow(2.0, static_cast<double>(step)/12.0) * _freq;
    generate();
  }

  void Sinusoid::compute_fft() {
    plan = fftw_plan_dft_1d(N_SAMPLES,
                                      signal, result,
                                      FFTW_FORWARD,
                                      FFTW_ESTIMATE);
    fftw_execute(plan);
  }

  void Sinusoid::freq_spec_to_dec_spec() {
  // Sampling frequency in Hz
  double Fs = 200.0;
  // Frequency delta: frequency distance between two succeding bins in FFT
  double dF = Fs / F_SAMPLING;
  // Sample time, duration in seconds processed in a single FFT pass
  double Ts = 1 / Fs;

    // Post-process frequency spectrum: transform to decibel
    std::vector<double> db_spectrum(N_BINS);
    for(int i = 0; i < N_BINS; i++){
       db_spectrum[i] = (20 *
                          log10(sqrt(  result[i][REAL] * result[i][REAL]
                                   + result[i][IMAG] * result[i][IMAG]))
                        );// / F_SAMPLING;
    }
    std::vector<double>::iterator max = std::max_element(db_spectrum.begin(),
                                                         db_spectrum.end());
    std::cout << "Detected frequency: "
              << std::distance(db_spectrum.begin(), max)
              << " Hz at velocity " << *max << std::endl;
  }

int main() {
  Sinusoid sine(1.0, 880.0);

  sine.generate();

  sine.compute_fft();
  sine.freq_spec_to_dec_spec();

  sine.shift_note_by_halftone(-3);

  sine.compute_fft();
  sine.freq_spec_to_dec_spec();

  return 0;
}

#endif
