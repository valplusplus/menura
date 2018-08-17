#include <fftw3.h>
#include <iostream>
#include <cmath>

#define REAL 0
#define IMAG 1

#define NUM_POINTS 44100

int main() {
  double * theta  = new double[NUM_POINTS];

  fftw_complex signal[NUM_POINTS];
  fftw_complex result[NUM_POINTS];

  FILE * gnuplot_pipe_s = popen ("gnuplot -persistent", "w");
  fprintf(gnuplot_pipe_s, "plot [0:882] '-' with lines\n");

  for (int i = 0; i < NUM_POINTS; ++i) {
    theta[i]        = (static_cast<double>(i)/static_cast<double>(NUM_POINTS)) *
                      2.0 * M_PI;
    signal[i][REAL] = 1.0 * sin(440.0  * theta[i]) +
                      0.5 * sin(523.25 * theta[i]) +
                      1.0 * sin(659.26 * theta[i]);
    signal[i][IMAG] = 0;

    // std::cout << signal[i][REAL] << std::endl;
    fprintf(gnuplot_pipe_s, "%hu %f\n", i, signal[i][REAL]);
  }
  fprintf(gnuplot_pipe_s, "e\n");
  fflush(gnuplot_pipe_s);

  fftw_plan plan = fftw_plan_dft_1d(NUM_POINTS,
                                    signal, result,
                                    FFTW_FORWARD,
                                    FFTW_ESTIMATE);
  fftw_execute(plan);

  FILE * gnuplot_pipe_f = popen ("gnuplot -persistent", "w");
  fprintf(gnuplot_pipe_f, "plot [350:750] '-' with lines\n");

  for(int i = 0; i < NUM_POINTS; i++) {
    // std::cout << result[i][REAL] << std::endl;
    fprintf(gnuplot_pipe_f, "%hu %f\n", i, result[i][REAL]);
  }

  fprintf(gnuplot_pipe_f, "e\n");
  fflush(gnuplot_pipe_f);

  fftw_destroy_plan(plan);
  delete[] theta;
  return 0;
}
