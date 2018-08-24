#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <string>

#define MINUS 0
#define PLUS  1
#define A4_IDX 57

using std::cout;
using std::cerr;
using std::endl;

std::vector<std::string> notes {
  "C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0",
  "C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
  "C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
  "C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
  "C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
  "C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
  "C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
  "C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
  "C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8",
  "C9","C#9","D9","D#9","E9","F9","F#9","G9","G#9","A9","A#9","B9"};

std::string note_of(double frequency, double pitch_hz = 440.0) {
  int note_idx = 12 * std::log2(frequency / pitch_hz) + A4_IDX;
  // cerr << "Detected note: " << notes[note_idx] << endl;

  double ideal_freq = pitch_hz * std::pow(2.0, (note_idx - A4_IDX) / 12.0);
  // cerr << "Ideal frequency: " << ideal_freq << endl;

  int cent_idx = 1200 * std::log2(frequency / ideal_freq);
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

  return notes[note_idx]
       + ((cent_idx >= 0) ? " (+"
                          : " (") + std::to_string(cent_idx) + " cents)";
}

int main(int argc, char * argv[]) {
  double frequency = 440.0;

  for (int argi = 1; argi < argc; ) {
    std::string flag(argv[argi]);
    if (flag == "-f") {
      frequency = std::stod(argv[argi+1]);
      argi += 2;
      continue;
    }
  }
  
  for(int i = 0; i < 10; i++) {
    cerr << "Frequency " << frequency << " Hz --> Note " << note_of(frequency) << endl;
    frequency += 100.0;
  }
  return 0;
}
