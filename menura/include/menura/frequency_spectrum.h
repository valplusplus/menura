#include <vector>

/**
 * Frequency Spectrum Concept
 * ===========================================================================
 *
 * Dependent Types/Concepts
 * ---------------------------------------------------------------------------
 *
 * spectrum_bin b:
 *   b.loudness();
 *   b.freqency();
 *
 *
 *
 * Frequency        f   : double
 * Loudness (dB)    l   : double
 * Freq. Spectrum   fs ~> Random-Access Sequence
 *
 * expression                      | semantics
 * ------------------------------- | -----------------------------------------
 * frequency_spectrum(it_a, it_b)  | Create frequency spectrum from sequence
 *                                   of audio input samples (represented as
 *                                   iterator range)
 * fs.begin(), fs.end()            | Random-access iterators on spectrum bins
 * fs[bin_idx]                     | Bin at given index in the spectrum
 * fs[f]                           | Loudness of given frequency in spectrum
 * fs[note]                        | Loudness of given note in spectrum,
 *                                   same as fs[frequency(note)]
 * fs.find_fundamental()           | Returns iterator to bin representing the
 *                                   fundamental frequency in spectrum
 *
 */



namespace menura {

/*   spectrum bin element:
 *
 *     auto bin_it = freq_spectrum.find_fundamental();
 *
 *     bin_it->db();
 *     bin_it->frequency();
 */
struct spectrum_bin {
  double db() const { return _db; }
  double frequency() const { return _spectrum.bin_index_to_freq(_bin_index); }
}


/*
 *   while (audio_istream.is_open()) {
 *     // read input samples required for a single
 *     // spectrum analysis
 *     audio_istream >> frequency_spectrum(1024) | dominant_freq() | note()
 *   }
 *
 */
class frequency_spectrum {

public:
  using const_iterator = typename std::vector<spectrum_bin>::const_iterator;

public:
  frequency_spectrum() = delete;
  frequency_spectrum(const audio_istream & ais, int num_bins)
    : _audio_in(ais)
    , _db_spectrum(num_bins)
  { }

  friend operator>>(audio_istream & audio_is, self_t & freq_spectrum);

  void analyze() {
    while((( err = Pa_IsStreamActive(stream)) == 1)) {
      data.frameIndex = 0;
      fftw_execute(plan);

      // Post-process frequency spectrum: transform to decibel
      for(int i = 0; i < NUM_BINS; i++){
         db_spectrum[i] = (20 *
                            log10(sqrt(  data.fftwOutput[i]
                                       * data.fftwOutput[i]))
                          );
      }
      std::vector<double>::iterator max = std::max_element(db_spectrum.begin(),
                                                           db_spectrum.end());
      auto bin_index = std::distance(db_spectrum.begin(), max);

      frequencies.push_back(freq);
      auto musical_note = menura::note_of(freq);
      if (interrupt % 67 == 0) {
        if (*max > 30) {
          std::cerr << "Frequency " << freq << " Hz"
                    << " --> Note " << musical_note
                    << " --> MIDI " << menura::midi_number_of(musical_note)
                    << " --> dB " << *max
                    << std::endl;
        }
      }
      interrupt++;
    }
  }

  // dB (~ volume) of given frequency in the spectrum
  double db(double freq) const {

  }



  const_iterator begin() const {
    return _db_spectrum.cbegin();
  }

  const_iterator end() const {
    return _db_spectrum.cend();
  }

private:
  double bin_index_to_freq(int i, double sample_rate) const {
    return static_cast<double>(i * (sample_rate / _db_spectrum.size() / 2));
  }

  int freq_to_bin_index(double freq, double sample_rate) {
    return std::round(freq / (sample_rate / _db_spectrum.size() / 2));
  }

private:
  std::vector<double> _db_spectrum;
};

} // namespace menura
