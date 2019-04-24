#!/bin/sh

# controllo il numero di parametri
if test $# -ne 1
then
	echo "Usage: $0 dir"
	exit 1
fi

# controllo che il parametro passato sia una cartella esistente e passabile
if test ! -d $1 -o ! -x $1
then
	echo "Cartella non esistente o non traversabile!"
fi

F=0 # contatore file
D=0 # contatore cartelle

for fname in *
do
	if test -d $fname
	then
		echo "[D] $fname"
		D=`expr $D + 1`
	fi
	if test -f $fname
	then
		echo "[F] $fname"
		F=`expr $F + 1`
	fi

done

echo "F=$F"
echo "D=$D"

exit 0
