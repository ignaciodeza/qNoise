CC=g++
CFLAGS= -std=gnu++11 -g -O3 -Wall -I../
DEPS = ../qNoise.h
OBJ = test_qNoise.o qNoise.o

clean:
	rm *.txt
	rm test_qNoise

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

qNoise.o: ../qNoise.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

test: $(OBJ)
	$(CC) $(CFLAGS) -o test_qNoise $^
	rm *.o
