#include <fftw3.h>
#include <iostream>
#include <cmath>

#include <vector>

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

  double * theta  = new double[N_SAMPLES];

  fftw_complex signal[N_SAMPLES];
  fftw_complex result[N_SAMPLES];

  FILE * gnuplot_pipe_s = popen ("gnuplot -persistent", "w");
  fprintf(gnuplot_pipe_s, "plot [0:882] '-' with lines\n");

  for (int i = 0; i < N_SAMPLES; ++i) {
    theta[i]        = (static_cast<double>(i)/static_cast<double>(N_SAMPLES)) *
                      2.0 * M_PI;
    signal[i][REAL] =   1.0 * sin(440.0  * theta[i])
                 //   + 0.5 * sin(523.25 * theta[i])
                 //   + 1.0 * sin(659.26 * theta[i])
                      ;
    signal[i][IMAG] = 0;

    fprintf(gnuplot_pipe_s, "%hu %f\n", i, signal[i][REAL]);
  }
  fprintf(gnuplot_pipe_s, "e\n");
  fflush(gnuplot_pipe_s);

  fftw_plan plan = fftw_plan_dft_1d(N_SAMPLES,
                                    signal, result,
                                    FFTW_FORWARD,
                                    FFTW_ESTIMATE);
  fftw_execute(plan);

  // Post-process frequency spectrum: transform to decibel
  std::vector<double> db_spectrum(N_BINS);
  for(int i = 0; i <= N_BINS; i++){
     db_spectrum[i] = (20 * 
                        log(sqrt(  result[i][REAL] * result[i][REAL]
                                 + result[i][IMAG] * result[i][IMAG]))
                      ) / F_SAMPLING;
  }

  FILE * gnuplot_pipe_f = popen ("gnuplot -persistent", "w");
  fprintf(gnuplot_pipe_f, "plot [350:750] '-' with lines\n");

  for(int i = 0; i < N_BINS; i++) {
    std::cout << db_spectrum[i] << std::endl;
    fprintf(gnuplot_pipe_f, "%hu %f\n", i, db_spectrum[i]);
  }

  fprintf(gnuplot_pipe_f, "e\n");
  fflush(gnuplot_pipe_f);

  fftw_destroy_plan(plan);
  delete[] theta;
  return 0;
}
