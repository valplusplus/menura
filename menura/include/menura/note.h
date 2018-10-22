#ifndef MENURA__NOTE_OF_FREQ_H__INCLUDED
#define MENURA__NOTE_OF_FREQ_H__INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include "pa_stream.h"


/**
 * Note Concept
 * ===========================================================================
 *
 * Note        n
 * Frequency   f : double
 * Pitch class p
 * Octave      o : uint8_t
 *
 * expression           | semantics
 * -------------------- | ----------------------------------------------------
 * note(p,o)            | Create node from pitch class and octave
 * note(f,ft)           | Create node from frequency f for tuning freq. ft
 * n1 != n2, n1 == n2   | Equality, inequality
 * n1 < n2, n1 > n2 ... | Sort comparison, based on pitch range (semitones)
 * octave(n)            | The note's octave
 * pitch_class(n)       | The note's pitch class
 * fundamental_freq(n)  | Fundamental frequency of a note, e.g. 440 for A4
 * chromatic_index(n)   | The note's offset in the chromatic domain, from
 *                        0 (lowest note: C-1) to 127 (B9), same as MIDI code
 * distance(n1, n2)     | Distance between notes, in semitones
 *
 */


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

  class note {
  public:
    friend std::ostream & operator<<(
                            std::ostream & lhs,
                            const menura::note & n);

  public:
    note() = delete;
    note(double frequency, double pitch_class = 440.0) {
      // start off from A4:
      *this = note(pitch_class::A, 4);

      if (frequency != 0) {
        *this += 12 * std::log2(frequency / pitch_hz);
      }
      double ideal_freq = pitch_hz
                          * std::pow(
                              2.0,
                              (note_idx - a4_idx) / 12.0);

      int cent_idx = 0;
      if(frequency != 0) {
        cent_idx = 1200 * std::log2(frequency / ideal_freq);
      }

      if(frequency >= ideal_freq) {
        if(cent_idx > 50) {
          *this++;
          cent_idx = 100 - cent_idx;
          if(cent_idx != 0)
            cent_idx = (-1) * cent_idx;
        }
      } else {
        if(cent_idx >= 50) {
          *this--;
          cent_idx = 100 - cent_idx;
        } else {
          if(cent_idx != 0)
            cent_idx = (-1) * cent_idx;
        }
      }
    }

    note(pitch p, uint8_t octave)
       : _pitch(p), _octave(octave) {  }

    note(const note & other)
       : _pitch(other.p), _octave(other.octave) {  }

    note & operator=(const note & rhs) {
      _pitch  = rhs.pitch;
      _octave = rhs.octave;
    }

    // Heighten note by given number of semitones
    note & operator+=(int semitone) {
       if (_pitch == B) {
         _octave++; _pitch = C;
       }
       else {
         _pitch = static_cast<pitch_class>(static_cast<int>(_pitch) + 1)
       }
       return *this;
    }
    note & operator++() {
      return *this += 1;
    }

    // Lower note by given number of semitones
    note & operator-=(int semitone) {
       if (_pitch == pitch_class::C) {
         _octave--;
         _pitch = pitch_class::B;
       }
       else {
         _pitch = static_cast<pitch_class>(static_cast<int>(_pitch) - 1)
       }
       return *this;
    }
    note & operator--() {
      return *this -= 1;
    }

    bool operator==(const note & rhs) const {
      return    _pitch  == rhs._pitch
             && _octave == rhs._octave;
    }
    bool operator!=(const note & rhs) const {
      return !(*this == rhs);
    }

    bool operator<(const note & rhs) const {
      return _octave < rhs._octave ||
             (   _octave == rhs._octave
              && _pitch   < rhs._pitch);
    }
    bool operator<=(const note & rhs) const {
      return *this == rhs || *this < rhs;
    }

    bool operator>(const note & rhs) const {
      return *this != rhs &&
             !(*this < rhs);
    }
    bool operator>=(const note & rhs) const {
      return *this == rhs || *this > rhs;
    }

  private:
     pitch_class  _pitch;
     uint8_t      _octave;
  };

  std::ostream & operator<<(std::ostream & lhs, const menura::note & n);

  int chromatic_index(const note & n);

  int distance(const note & a, const & note b) {
    return chromatic_index(b) - chromatic_index(a);
  }

} // namespace menura

#endif // MENURA__NOTE_OF_FREQ_H__INCLUDED
