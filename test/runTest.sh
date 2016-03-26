 #!/bin/bash
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
