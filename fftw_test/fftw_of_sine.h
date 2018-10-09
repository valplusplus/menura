#ifndef FFT_OF_SINE_H_
#define FFT_OF_SINE_H_

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

class Sinusoid {
public:
  Sinusoid() = default;
  Sinusoid(double A, double freq)
  : _A(A)
  , _freq(freq) {}
  ~Sinusoid() {
    fftw_destroy_plan(plan);
  }
  void generate();
  // skipping several steps here and pretending the correlation between
  // frequency and note to be already known:
  void shift_note_by_halftone(int step);
  void compute_fft();
  void freq_spec_to_dec_spec();

private:
  double _A    = 1.0;
  double _freq = 440.0;
  fftw_complex signal[N_SAMPLES];
  fftw_complex result[N_SAMPLES];
  fftw_plan plan;
};

#endif
