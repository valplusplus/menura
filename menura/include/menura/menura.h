/*
 *
 *  
 *
 *
 *
 *
 *
 */


struct note_t {
  int velocity;
  note_value value;
  time_start;
  time_end;
};


int main() {
  const int fft_window_size = 1024;

  std::vector<wav_sample_t> wave_samples;
  wave_samples.resize(fft_window_size);

  std::vector<freq_t> freq_spectrum;
  freq_spectrum.resize(fft_window_size);

  std::thread sampling_thread { 
    // Sampling loop
    while(running) {
      // Loop frequency determined by audio sampling
      // (portaudio):
      read_samples_from_mic(wave_samples.begin(),
                            wave_samples.size());

      signal_processing_thread();
    }
  }

  std::thread processing_thread {
    // Processing loop
    note_t detected_note;
    while(running) {
      wait_for_signal()

      // Loop frequency determined by duration of
      // processing:
      fft_from_samples(wave_samples,
                       freq_spectrum.begin(),
                       freq_spectrum.size());

      freq_t dom_freg  = resolve_dominant_freq(freq_spectrum);
      note_t curr_note = freq_to_note(dom_freq);

      if (detected_note != curr_note) {
        write_note_to_midi(detected_note,
                           time_note_start,
                           time_now());
        detected_note = curr_note;
      } else {
        // Still "hearing" same note
      }

      // Time elapsed until here must not exceed
      // sampling loop period
    }
  }

  return 0;
}

