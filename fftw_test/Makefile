
CC=clang++
CFLAGS=--std=c++11 -Wall -Wextra -pedantic -g -I$TACC_FFTW3_INC
LDFLAGS=-lpthread -L$TACC_FFTW3_LIB -lfftw3

INCLUDES=-I${GTEST_BASE}/include -I../

OBJS = 		fft_test.o

.DEFAULT: all
all: $(OBJS) fft_test.bin

run: clean fft_test.bin
	./fft_test.bin

%.o: %.c
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cc
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.bin: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	-rm -f *.o
	-rm -f *.a
	-rm -f *.bin
	-rm -f ../solution/*.o
	-rm -f ../solution/*.a
