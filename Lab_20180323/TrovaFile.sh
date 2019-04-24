#!/bin/sh

# controllo il numero dei parametri
if test $# -ne 1
then
	echo "Usage: $0 f_name"
	exit 1
fi

# controllo che il parametro sia un nome relativo semplice
case $1 in
	*/*) echo "$1 non è un nome relativo semplice"
		exit 1
		;;
	*) ;;
esac

if test -f $1
then
	echo Il file $1 si trova in `pwd`
	exit 0
else
	echo Il file $1 non è presente nella directory corrente
	exit 1
fi


exit 0
