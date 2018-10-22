
#ifndef MENURA__PA_STREAM_H__INCLUDED
#define MENURA__PA_STREAM_H__INCLUDED


#include <stdio.h>
#include <stdlib.h>

#include "portaudio.h"

#include <fftw3.h>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <vector>
#include <iterator>


/**
 * Note Concept
 * ===========================================================================
 *
 * Program Options      opts
 * Audio Sample         s
 * Sample Buffer        buf   ~> Random-Access Sequence of Audio Sample values
 * Audio Input Stream   ais
 *
 *
 * expression           | semantics
 * -------------------- | ----------------------------------------------------
 * audio_istream(opts)  | Configure audio input stream from options, e.g.
 *                        sample rate (defaults to 44100)
 * ais >> buf           | Reads buf.size() samples from audio input and fills
 *                        buffer starting from its first element, overwriting
 *                        existing values in buffer
 * ais.is_opened()      | Whether the input stream is ready
 * ais.close()          | Explicitly flush and close the input stream,
 *                        implicitly called in destruction of ais
 *
 */



namespace menura {

/* Select sample format. */
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"

typedef struct
{
    int      frameIndex;
    int      maxFrameIndex;
    double * recordedSamples;
    double * fftwOutput;
}
pa_sampling_data;

class audio_istream {
public:
  audio_istream() = delete;
  audio_istream(const menura::program_options & opts);

  ~audio_istream();

  bool open();
  bool record();
  void analyze();

  std::vector<double> frequencies();
  static int recordCallback( const void * inputBuffer
                           , void * outputBuffer
                           , unsigned long framesPerBuffer
                           , const PaStreamCallbackTimeInfo * timeInfo
                           , PaStreamCallbackFlags statusFlags
                           , void *userData
                           );
private:

  PaStreamParameters  inputParameters;
  PaStream *          stream;
  std::vector<double> frequencies;
  PaError             err              = paNoError;
  pa_sampling_data    data;
  bool                initialized;
  int                 totalFrames;
  int                 numSamples;
  int                 numBytes;
  fftw_plan           plan;
  SAMPLE              max, val;
  double              average;

private:
  void log_port_audio_rc(int err) {
    if (err != paNoError) {
      std::cerr << "ERROR: PortAudio: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
    }
  }
};

} // namespace menura

#endif // MENURA__PA_STREAM_H__INCLUDED
