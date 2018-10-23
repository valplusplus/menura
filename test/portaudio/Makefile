
CC=clang++
CFLAGS=--std=c++11 -Wall -Wextra -pedantic -g -I$TACC_FFTW3_INC
LDFLAGS=-lpthread -L$TACC_FFTW3_LIB -lfftw3 -lrt -lm -lasound -ljack -pthread -lportaudio


INCLUDES=-I${GTEST_BASE}/include -I../

OBJS = 		../../menura/src/note.o \
			 ../../menura/src/audio_istream.o

.DEFAULT: all
all: $(OBJS) record.bin

run: clean record.bin
	./record.bin

%.o: %.c
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.bin: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f *.o
	-rm -f *.bin
	#-rm -f generated.txt
	#-rm -f recorded.txt
	#-rm -f analyzed.txt
