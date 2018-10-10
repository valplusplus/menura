#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "pa_stream.h"

namespace menura {

enum pitch_class {
  C,       // B_#, D_bb
  C_sharp, // D_b, B_x
  D,       // E_bb, C_x
  D_sharp, // E_b, F_bb
  E,       // F_b, D_x
  F,       // E_#, G_bb
  F_sharp, // G_b, E_x
  G,       // F_x, A_bb
  G_sharp, // A_b
  A,       // G_x, B_bb
  A_sharp, // B_b, C_bb
  B        // A_x, C_b
};

// enum accidental {
//   natural,
//   sharp,        // #
//   flat,         // b
//   double_sharp, // x
//   double_flat   // bb
// };

struct note {
  pitch_class name;   // C, D, E, ...
  bool accidental;    // natural, sharp
  uint8_t octave;     // 0-9

  note(pitch_class n_name, bool n_acc, uint8_t n_oct)
  : name(n_name)
  , accidental(n_acc)
  , octave(n_oct) {}

  bool operator==(const note & rhs) const {
    return (name == rhs.name
         && accidental == rhs.accidental
         && octave == rhs.octave);
  }
};

std::ostream& operator<<(std::ostream& lhs, menura::note n) {
  switch(n.name) {
    case C:       lhs << "C";  break;
    case C_sharp: lhs << "C#"; break;
    case D:       lhs << "D";  break;
    case D_sharp: lhs << "D#"; break;
    case E:       lhs << "E";  break;
    case F:       lhs << "F";  break;
    case F_sharp: lhs << "F#"; break;
    case G:       lhs << "G";  break;
    case G_sharp: lhs << "G#"; break;
    case A:       lhs << "A";  break;
    case A_sharp: lhs << "A#"; break;
    case B:       lhs << "B";  break;
  }
  return lhs << static_cast<int>(n.octave);
}

std::vector<note> notes;
void note_setup() {
  bool accidental = 0;
  for(uint8_t octave = 0; octave <= 9; octave++) {
    for(int pitch = 0; pitch <= 11; ++pitch) {
      notes.emplace_back(static_cast<menura::pitch_class>(pitch), accidental, octave);
      if(pitch != 4 && pitch != 11) {
        accidental = !accidental;
      }
    }
  }
}

int midi_number_of(note n) {
  auto note_idx = std::find(std::begin(notes), std::end(notes), n);
  int key = std::distance(notes.begin(), note_idx) + 12;

  // std::cout << "Note "       << n
  //           << " --> MIDI " << key
  //           << std::endl;
  return key;
}

note note_of(double frequency, double pitch_hz = 440.0) {
  int a4_idx = 57;
  int note_idx = a4_idx;
  if(frequency != 0) {
    note_idx = 12 * std::log2(frequency / pitch_hz) + a4_idx;
  }
  // cerr << "Detected note: " << notes[note_idx] << endl;

  double ideal_freq = pitch_hz * std::pow(2.0, (note_idx - a4_idx) / 12.0);
  // cerr << "Ideal frequency: " << ideal_freq << endl;

  int cent_idx = 0;
  if(frequency != 0) {
    cent_idx = 1200 * std::log2(frequency / ideal_freq);
  }
  // cerr << "Difference in cents: " << cent_idx << endl;

  if(frequency >= ideal_freq) {
    if(cent_idx > 50) {
      note_idx++;
      // cerr << "Closest note: " << notes[note_idx] << endl;
      cent_idx = 100 - cent_idx;
      if(cent_idx != 0)
        cent_idx = (-1) * cent_idx;
    }
  } else {
    if(cent_idx >= 50) {
      note_idx--;
      // cerr << "Closest note: " << notes[note_idx] << endl;
      cent_idx = 100 - cent_idx;
    } else {
      if(cent_idx != 0)
        cent_idx = (-1) * cent_idx;
    }
  }

  // std::cout << notes[note_idx]
  //           << ((cent_idx >= 0) ? " (+"
  //                               : " (")
  //           << cent_idx << " cents)"
  //           << std::endl;

  return notes[note_idx];
}
}

int main() {
  menura::note_setup();

  menura::PortAudio stream;

  stream.init();
  stream.open();
  stream.record();
  stream.analyze();

  auto freqs = stream.detected_frequencies();

  for(int i = 0; i < freqs.size(); i++) {
    auto musical_note = menura::note_of(freqs[i]);
    std::cerr << "Frequency " << freqs[i] << " Hz"
              << " --> Note " << musical_note
              << " --> MIDI " << menura::midi_number_of(musical_note)
              << std::endl;
  }
  return 0;
}
