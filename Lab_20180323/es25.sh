#!/bin/sh

C=0 # numero argomenti
LIST= # lista degli argomenti escluso l'ultimo
LAST= # contiene l'ultimo argomento

if test $# -eq 0
then
	echo Nessun argomento passato
	exit 1
fi

for arg
do
	C=`expr $C + 1`
	if test $C -eq $#
	then
		LAST=$arg
	else
		LIST="$LIST $arg"
	fi
done

echo Passati $# argomenti
echo "L'ultimo è $LAST"
echo "I primi `expr $# - 1` sono $LIST"


exit 0
