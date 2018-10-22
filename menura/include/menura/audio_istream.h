/*
 * $Id$
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however,
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also
 * requested that these non-binding requests be included along with the
 * license above.
 */

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

namespace menura {

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define MAX_BPM    (120)
#define N_SAMPLES  (SAMPLE_RATE / (MAX_BPM / 60 * 2))
#define N_BINS     (N_SAMPLES/2)-1
#define NUM_SECONDS     (1)
#define NUM_CHANNELS    (1)
#define NUM_SAMPLES    (NUM_SECONDS * SAMPLE_RATE * NUM_CHANNELS)
#define NUM_BINS     (NUM_SAMPLES/2)-1

/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#endif

typedef struct
{
    int      frameIndex;
    int      maxFrameIndex;
    double * recordedSamples;
    double * fftwOutput;
}
paTestData;

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
  PaError             err = paNoError;
  paTestData          data;
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
