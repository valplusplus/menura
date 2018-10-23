#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
// #include "../include/menura/audio_istream.h"
#include "../include/menura/note.h"

namespace menura {

// std::vector<note> notes;

std::ostream& operator<<(std::ostream& lhs, const menura::note & n) {
  switch(n._pitch) {
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
  return lhs << static_cast<int>(n._octave);
}

// void note_setup() {
//   bool accidental = 0;
//   for(uint8_t octave = 0; octave <= 9; octave++) {
//     for(int pitch = 0; pitch <= 11; ++pitch) {
//       notes.emplace_back(static_cast<menura::pitch_class>(pitch), accidental, octave);
//       if(pitch != 4 && pitch != 11) {
//         accidental = !accidental;
//       }
//     }
//   }
// }

// int midi_number_of(note n) {
//   auto note_idx = std::find(std::begin(notes), std::end(notes), n);
//   int key = std::distance(notes.begin(), note_idx) + 12;
//
//   // std::cout << "Note "       << n
//   //           << " --> MIDI " << key
//   //           << std::endl;
//   return key;
// }

// note note_of(double frequency, double pitch_hz) {
//   int a4_idx = 57;
//   int note_idx = a4_idx;
//   if(frequency != 0) {
//     note_idx = 12 * std::log2(frequency / pitch_hz) + a4_idx;
//   }
//   // cerr << "Detected note: " << notes[note_idx] << endl;
//
//   double ideal_freq = pitch_hz * std::pow(2.0, (note_idx - a4_idx) / 12.0);
//   // cerr << "Ideal frequency: " << ideal_freq << endl;
//
//   int cent_idx = 0;
//   if(frequency != 0) {
//     cent_idx = 1200 * std::log2(frequency / ideal_freq);
//   }
//   // cerr << "Difference in cents: " << cent_idx << endl;
//
//   if(frequency >= ideal_freq) {
//     if(cent_idx > 50) {
//       note_idx++;
//       // cerr << "Closest note: " << notes[note_idx] << endl;
//       cent_idx = 100 - cent_idx;
//       if(cent_idx != 0)
//         cent_idx = (-1) * cent_idx;
//     }
//   } else {
//     if(cent_idx >= 50) {
//       note_idx--;
//       // cerr << "Closest note: " << notes[note_idx] << endl;
//       cent_idx = 100 - cent_idx;
//     } else {
//       if(cent_idx != 0)
//         cent_idx = (-1) * cent_idx;
//     }
//   }
//
//   // std::cout << notes[note_idx]
//   //           << ((cent_idx >= 0) ? " (+"
//   //                               : " (")
//   //           << cent_idx << " cents)"
//   //           << std::endl;
//
//   return notes[note_idx];
// }

} // namespace menura
