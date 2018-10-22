namespace menura {
￼
￼ class frequency_spectrum {
￼
￼ public:
￼   frequency_spectrum() = delete;
￼   frequency_spectrum(const audio_istream & ais)
￼     : _audio_in(ais)
￼   { }
￼
￼   void analyze() {
￼     int interrupt = 0;
￼     std::vector<double> db_spectrum(NUM_BINS);
￼
￼     while((( err = Pa_IsStreamActive(stream)) == 1) && (interrupt < 2000)) {
￼       data.frameIndex = 0;
￼       fftw_execute(plan);
￼
￼       // Post-process frequency spectrum: transform to decibel
￼       for(int i = 0; i < NUM_BINS; i++){
￼          db_spectrum[i] = (20 *
￼                             log10(sqrt(data.fftwOutput[i] * data.fftwOutput[i]))
￼                           );
￼       }
￼       std::vector<double>::iterator max = std::max_element(db_spectrum.begin(),
￼                                                            db_spectrum.end());
￼       auto freq = std::distance(db_spectrum.begin(), max);
￼       frequencies.push_back(freq);
￼       auto musical_note = menura::note_of(freq);
￼       if (interrupt % 67 == 0) {
￼         if (*max > 30) {
￼           std::cerr << "Frequency " << freq << " Hz"
￼                     << " --> Note " << musical_note
￼                     << " --> MIDI " << menura::midi_number_of(musical_note)
￼                     << " --> dB " << *max
￼                     << std::endl;
￼         }
￼       }
￼       interrupt++;
￼     }
￼   }
￼ };
￼
￼ } // namespace menura
￼
