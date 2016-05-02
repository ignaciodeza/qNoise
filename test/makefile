#Makefile for test file in qNoise
#qNoise: A generator of non-Gaussian colored noise
#Copyright © 2015, Juan Ignacio Deza
#email: juan.ignacio.deza@upc.edu
# Licence
#  This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


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
