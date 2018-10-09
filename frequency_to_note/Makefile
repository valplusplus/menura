
CC=clang++
CFLAGS=--std=c++11 -Wall -Wextra -pedantic -g
LDFLAGS=-lpthread -L${GTEST_BASE}/lib -lgtest

INCLUDES=-I${GTEST_BASE}/include -I../

OBJS = 		note_of_freq.o

.DEFAULT: all
all: $(OBJS) fton_test.bin

run: clean fton_test.bin
	./fton_test.bin

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
