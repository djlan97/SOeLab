#!/bin/sh
#file comandi leggi e mostra
#CONTROLLO SU PRESENZA DI UN PARAMETRO DIVERSO DA PRIMA
if test -z $1
then 	echo Errore: voglio un parametro
	exit 1
fi

for fname
do
	if test ! -f $fname -o ! -r $fname
	then 	echo Errore: $fname non file oppure non leggibile
		exit 2
	fi
	echo "vuoi visualizzare il file $fname (si/no)?"
	read var1
	if test $var1 = si
	then 
		ls -la $fname; cat $fname
	else echo niente stampa di $fname
	fi
done

