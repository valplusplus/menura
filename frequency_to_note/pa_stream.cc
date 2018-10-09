#include "pa_stream.h"

namespace menura {

PortAudio::PortAudio()
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
}

PortAudio::~PortAudio() {
  if (initialized) {
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
      std::cerr << "An error occured while closing the PortAudio stream: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
    }
    Pa_Terminate();
    fftw_destroy_plan(plan);
  }
}

bool PortAudio::init() {
  err = Pa_Initialize();
  if (err != paNoError) {
    std::cerr << "An error occured while initializing PortAudio: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
    return false;
  }
  initialized = true;
  return true;
}

bool PortAudio::open() {
  inputParameters.device = Pa_GetDefaultInputDevice();
  if (inputParameters.device == paNoDevice) {
      std::cerr << "Error: No default input device.\n";
      return false;
  }
  inputParameters.channelCount = 1;                    /* mono input */
  inputParameters.sampleFormat = PA_SAMPLE_TYPE;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)
                                   ->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  return true;
}

bool PortAudio::record() {
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
      std::cerr << "An error occured while opening the PortAudio stream: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
      return false;
  }

  err = Pa_StartStream( stream );
  if( err != paNoError )  {
    Pa_Terminate();
    if( data.recordedSamples )       /* Sure it is NULL or valid. */
      free( data.recordedSamples );
      std::cerr << "An error occured while starting the PortAudio stream: "
                << Pa_GetErrorText(err) << " (" << err << ")\n";
      return false;
  }

  std::cout << "\n=== Now recording!!"
            << "Please whistle into the microphone. ===\n";

  return true;
}

void PortAudio::analyze() {
  int interrupt = 0;
  std::vector<double> db_spectrum(NUM_BINS);
  // fftw_data.open("analyzed.txt");
  // fftw_data << "Hz\t dB" << std::endl;

  while((( err = Pa_IsStreamActive(stream)) == 1) && (interrupt < 200)) {
    data.frameIndex = 0;
    fftw_execute(plan);

    // Post-process frequency spectrum: transform to decibel
    for(int i = 0; i < NUM_BINS; i++){
       db_spectrum[i] = (20 *
                          log10(sqrt(data.fftwOutput[i] * data.fftwOutput[i]))
                        ) / SAMPLE_RATE;
    }
    std::vector<double>::iterator max = std::max_element(db_spectrum.begin(),
                                                         db_spectrum.end());
    frequencies.push_back(std::distance(db_spectrum.begin(), max));
    // std::cout << "index = " << data.frameIndex << std::endl;
    interrupt++;
  }
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int PortAudio::recordCallback( const void *inputBuffer, void *outputBuffer,
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

std::vector<double> PortAudio::detected_frequencies() {
  return frequencies;
}

}
