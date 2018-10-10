#ifndef MENURA__NOTE_OF_FREQ_H__INCLUDED
#define MENURA__NOTE_OF_FREQ_H__INCLUDED

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

  std::ostream& operator<<(std::ostream& lhs, menura::note n);

  void note_setup();

  int midi_number_of(note n);

  note note_of(double frequency, double pitch_hz = 440.0);

  static std::vector<note> notes;

} // namespace menura

#endif // MENURA__NOTE_OF_FREQ_H__INCLUDED
