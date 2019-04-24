#!/bin/sh

# controllo il numero di parametri
if test $# -ne 2
then
	echo "Usage: $0 f_sorg f_dest"
	exit 1
fi

# controllo che sia un nome relativo semplice
for fname
do
	case $fname in
		*/*) echo "$fname non è un nome relativo semplice!"
			exit 2
			;;
		*) ;;
	esac
done

# controllo che il primo file esista e sia leggibile
if test ! -f $1 -o ! -r $1
then
	echo "$1 non esiste oppure non è leggibile!"
	exit 3
fi

# controllo che il secondo file esista e sia scrivibile
if test -f $2
then
	if ! -w $2
	then
		echo "$2 non è scrivibile!"
		exit 4
	fi


else
	if -w
	then
		echo "$2 non esiste e la cartella corrente non è scrivibile!"
		exit 5
	fi

fi

cat < $1 >> $2

exit 0
