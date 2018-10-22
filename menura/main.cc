
#include <unistd.h>

#include "note_of_freq.h"

int main() {
  menura::note_setup();

  menura::PortAudio stream;

  stream.init();
  stream.open();
  stream.record();
  stream.analyze();

  auto freqs = stream.detected_frequencies();

  return EXIT_SUCCESS;

  for(int i = 0; i < freqs.size(); i++) {
    auto musical_note = menura::note_of(freqs[i]);
    std::cerr << "Frequency " << freqs[i] << " Hz"
              << " --> Note " << musical_note
              << " --> MIDI " << menura::midi_number_of(musical_note)
              << std::endl;
  }
  return 0;
}
