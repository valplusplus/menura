#include "pa_stream.h"
#include "note_of_freq.h"

namespace menura {

  audio_istream::audio_istream()
  : totalFrames(NUM_SECONDS * SAMPLE_RATE)
  , numSamples(totalFrames * NUM_CHANNELS)
  , numBytes(numSamples * sizeof(double))
  {
    data.maxFrameIndex = totalFrames; /* Record for a few seconds. */
    data.frameIndex = 0;

    data.recordedSamples = (double *) malloc(numBytes); /* From now on,
                                       recordedSamples is initialised. */
    data.fftwOutput = (double *) malloc(numBytes);
    plan = fftw_plan_r2r_1d(NUM_SAMPLES,
                            data.recordedSamples,
                            data.fftwOutput,
                            FFTW_R2HC,
                            FFTW_ESTIMATE);
    err = Pa_Initialize();
    if (err != paNoError) {
      std::cerr << "ERROR: initializing audio_istream: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
    }
    initialized = true;
  }

  audio_istream::~audio_istream() {
    if (initialized) {
      err = Pa_CloseStream(stream);
      log_port_audio_rc(err);
      Pa_Terminate();
      fftw_destroy_plan(plan);
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
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(
                                         inputParameters.device
                                       )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    return true;
  }

  bool audio_istream::record() {
    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  /* &outputParameters, */
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't
                                                       bother clipping them */
              recordCallback,
              &data );
    if( err != paNoError ) {
      Pa_Terminate();
      if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
        std::cerr << "An error occured while opening the audio_istream stream: "
                  << Pa_GetErrorText(err) << " (" << err << ")\n";
        return false;
    }

    err = Pa_StartStream( stream );
    if( err != paNoError )  {
      Pa_Terminate();
      if( data.recordedSamples )       /* Sure it is NULL or valid. */
        free( data.recordedSamples );
        std::cerr << "An error occured while starting the audio_istream stream: "
                  << Pa_GetErrorText(err) << " (" << err << ")\n";
        return false;
    }

    std::cout << "\n=== Now recording!!"
              << "Please whistle into the microphone. ===\n";

    return true;
  }


  /* This routine will be called by the audio_istream engine when audio is needed.
  ** It may be called at interrupt level on some machines so don't do anything
  ** that could mess up the system like calling malloc() or free().
  */
  int audio_istream::recordCallback( const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData )
  {
      paTestData *data = (paTestData*)userData;
      const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
      double *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
      long framesToCalc;
      long i;
      int finished;
      unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

      (void) outputBuffer; /* Prevent unused variable warnings. */
      (void) timeInfo;
      (void) statusFlags;
      (void) userData;

      if( framesLeft < framesPerBuffer )
      {
          framesToCalc = framesLeft;
          finished = paComplete;
      }
      else
      {
          framesToCalc = framesPerBuffer;
          finished = paContinue;
      }

      if( inputBuffer == NULL )
      {
          for( i=0; i<framesToCalc; i++ )
          {
              *wptr++ = SAMPLE_SILENCE;  /* left */
              if( NUM_CHANNELS == 2 ) *wptr++ = SAMPLE_SILENCE;  /* right */
          }
      }
      else
      {
          for( i=0; i<framesToCalc; i++ )
          {
              *wptr++ = *rptr++;  /* left */
              if( NUM_CHANNELS == 2 ) *wptr++ = *rptr++;  /* right */
          }
      }
      data->frameIndex += framesToCalc;

      return finished;
  }

  std::vector<double> audio_istream::frequencies() {
    return frequencies;
  }

} // namespace menura
