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

std::string fton(const double frequency) {
  double r = std::pow(2.0, 1.0/12.0);
  double cent = std::pow(2.0, 1.0/1200.0);
  int r_index = 0;
  int cent_index = 0;
  int side;

  double concert_pitch = 440.0;

  if(frequency >= concert_pitch) {
   while(frequency >= r*concert_pitch) {
     concert_pitch = r*concert_pitch;
     r_index++;
   }
   while(frequency > cent*concert_pitch) {
     concert_pitch = cent*concert_pitch;
     cent_index++;
   }
   if((cent*concert_pitch - frequency) < (frequency - concert_pitch))
     cent_index++;
   if(cent_index > 50) {
     r_index++;
     cent_index = 100 - cent_index;
     if(cent_index != 0)
       side = MINUS;
     else
       side = PLUS;
   }
   else
     side = PLUS;
  }

  else {
   while(frequency <= concert_pitch/r) {
     concert_pitch = concert_pitch/r;
     r_index--;
   }
   while(frequency < concert_pitch/cent) {
     concert_pitch = concert_pitch/cent;
     cent_index++;
   }
   if((frequency - concert_pitch/cent) < (concert_pitch - frequency))
     cent_index++;
   if(cent_index >= 50) {
     r_index--;
     cent_index = 100 - cent_index;
     side = PLUS;
   }
   else {
     if(cent_index != 0)
       side = MINUS;
     else
       side = PLUS;
   }
  }

  auto result = notes[A4_IDX + r_index];
  if(side == PLUS)
   result = result + " (+";
  else
   result = result + " (-";
  result = result + std::to_string(cent_index) + " cents)";
  return result;
}

int main(int argc, char * argv[]) {
  double frequency = 440.0;

  for (int argi = 1; argi < argc; ) {
    std::string flag(argv[argi]);
    if (flag == "-f") {
      frequency = std::atof(argv[argi+1]);
      argi += 2;
      continue;
    }
  }

  auto result = fton(frequency);
  cerr << "Frequency " << frequency << "Hz --> Note " << result << endl;

  return 0;
}
