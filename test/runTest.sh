#!/bin/bash

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
ERROR=0
H=0.01
N=$1
if [ "$N" = "" ]; then
	N=10
fi

q=0.7
tau=1
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
tau=10
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=100
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=1
q=1
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=10
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=100
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=1
q=1.3
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=10
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))

tau=100
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))


echo "-----------------------------------------"
if [ "$ERROR" = "0" ]; then
	echo "All tests OK."
	true
else
	echo "There were errors."
	false
fi
