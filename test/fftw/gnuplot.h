#ifndef GNUPLOT_H_
#define GNUPLOT_H_

#include <string>
#include <iostream>

class Gnuplot {
public:
  Gnuplot() {
    gnuplotpipe= popen("gnuplot -persist","w");
    if (!gnuplotpipe) {
      std::cerr<< ("Gnuplot not found !");
    }
  }

  ~Gnuplot() {
    fprintf(gnuplotpipe,"exit\n");
    pclose(gnuplotpipe);
  }

  void plot_xrange(const int first, const int last) const {
    std::string command = "plot [" + std::to_string(first)
                        + ":"      + std::to_string(last)
                        + "] '-' with lines";
    fprintf(gnuplotpipe,"%s\n",command.c_str());
    fflush(gnuplotpipe);
  }

  void write_data(const int idx, const double val) const {
    fprintf(gnuplotpipe,"%d %f\n", idx, val);
  }

protected:
  FILE * gnuplotpipe;
};

#endif
