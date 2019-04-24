#!/bin/sh
#file comandi leggi e mostra
#CONTROLLO SU PRESENZA DI UN PARAMETRO DIVERSO DA PRIMA
if test -z $1
then 	echo Errore: voglio un parametro
	exit 1
fi
if test ! -f $1 -o ! -r $1
then 	echo Errore: $1 non file oppure non leggibile
	exit 2
fi
echo "vuoi visualizzare il file $1 (si/no)?"
read var1
if test $var1 = si
then 
	ls -la $1; cat $1
else echo niente stampa di $1
fi

