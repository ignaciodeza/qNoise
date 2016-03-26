#Makefile for test file in qNoise
#qNoise: A generator of non-Gaussian colored noise
#Copyright (c) 2015, Juan Ignacio Deza
#email: juan.ignacio.deza@upc.edu

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
