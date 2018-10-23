// #include "../include/menura/note.h"
#include "../include/menura/audio_istream.h"

namespace menura {

audio_istream::audio_istream(const menura::program_options & opts)
    : _opts(opts),
      totalFrames(opts.num_seconds * opts.sample_rate),
      numSamples(totalFrames * opts.num_channels),
      numBytes(numSamples * sizeof(double)) {
  _sampling_data.maxFrameIndex = totalFrames; /* Record for a few seconds. */
  _sampling_data.frameIndex = 0;

  _sampling_data.recordedSamples = (double *)malloc(numBytes); /* From now on,
                                    recordedSamples is initialised. */
  _sampling_data.fftwOutput = (double *)malloc(numBytes);
  plan = fftw_plan_r2r_1d(opts.num_samples, _sampling_data.recordedSamples, _sampling_data.fftwOutput,
                          FFTW_R2HC, FFTW_ESTIMATE);
  err = Pa_Initialize();
  if (err != paNoError) {
    std::cerr << "ERROR: initializing audio_istream: " << Pa_GetErrorText(err)
              << " (" << err << ")\n";
  }
  initialized = true;
}

audio_istream::~audio_istream() {
  if (initialized) {
    err = Pa_CloseStream(stream);
    log_port_audio_rc(err);
    Pa_Terminate();
  }
}

bool audio_istream::open() {
  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice) {
    std::cerr << "Error: No default input device.\n";
    return false;
  }
  inputParameters.channelCount = 1;
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency =
      Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  return true;
}

bool audio_istream::record() {
  /* Record some audio. -------------------------------------------- */
  err =
      Pa_OpenStream(&stream, &inputParameters, NULL, /* &outputParameters, */
                    _opts.sample_rate, _opts.frames_per_buffer,
                    paClipOff, /* we won't output out of range samples so don't
                                                        bother clipping them */
                    recordCallback, &_sampling_data);
  if (err != paNoError) {
    Pa_Terminate();
    if (_sampling_data.recordedSamples) /* Sure it is NULL or valid. */
      free(_sampling_data.recordedSamples);
    std::cerr << "An error occured while opening the audio_istream stream: "
              << Pa_GetErrorText(err) << " (" << err << ")\n";
    return false;
  }

  err = Pa_StartStream(stream);
  if (err != paNoError) {
    Pa_Terminate();
    if (_sampling_data.recordedSamples) /* Sure it is NULL or valid. */
      free(_sampling_data.recordedSamples);
    std::cerr << "An error occured while starting the audio_istream stream: "
              << Pa_GetErrorText(err) << " (" << err << ")\n";
    return false;
  }

  std::cout << "\n=== Now recording!!"
            << "Please whistle into the microphone. ===\n";

  return true;
}

/* This routine will be called by the audio_istream engine when audio is
 * requested.
 * It may be called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */
int audio_istream::recordCallback(const void    *inputBuffer,
                                  void          *outputBuffer,
                                  unsigned long  framesPerBuffer,
                                  const PaStreamCallbackTimeInfo *timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void          *userData) {
  pa_sampling_data *_sampling_data = (pa_sampling_data *)userData;
  const SAMPLE *rptr = (const SAMPLE *)inputBuffer;
  double *wptr = &_sampling_data
                    ->recordedSamples[
                      _sampling_data->frameIndex * numChannels];
  long framesToCalc;
  long i;
  int finished;
  unsigned long framesLeft =   _sampling_data->maxFrameIndex
                             - _sampling_data->frameIndex;

  (void)outputBuffer; /* Prevent unused variable warnings. */
  (void)timeInfo;
  (void)statusFlags;
  (void)userData;

  if (framesLeft < framesPerBuffer) {
    framesToCalc = framesLeft;
    finished = paComplete;
  } else {
    framesToCalc = framesPerBuffer;
    finished = paContinue;
  }

  if (inputBuffer == NULL) {
    for (i = 0; i < framesToCalc; i++) {
      *wptr++ = SAMPLE_SILENCE; /* left */
      if (numChannels == 2)
        *wptr++ = SAMPLE_SILENCE; /* right */
    }
  } else {
    for (i = 0; i < framesToCalc; i++) {
      *wptr++ = *rptr++; /* left */
      if (numChannels == 2)
        *wptr++ = *rptr++; /* right */
    }
  }
  _sampling_data->frameIndex += framesToCalc;

  return finished;
}

std::vector<double> audio_istream::frequencies() { return freqs; }

} // namespace menura
