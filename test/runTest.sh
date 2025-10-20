#!/bin/bash

#qNoise: A generator of non-Gaussian colored noise
#Copyright © 2021, Juan Ignacio Deza
#email: ignacio.deza@uwe.ac.uk
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

tau=0.1
q=0.1
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.2
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.3
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.4
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.5
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.6
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.7
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.8
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=0.9
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=1.0
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_1_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=1.1
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=1.2
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
q=1.3
echo "-----------------------------------------"
echo "tau=$tau; H=$H; q=$q; N=$N"
echo "-----------------------------------------"
./test_qNoise $tau $H $q $N
ERROR=$(($ERROR+$?))
cat 'qNoise_'"$tau"'_'"$H"'_'"$q"'_'"$N"'.txt'
ERROR=$(($ERROR+$?))
