#!/bin/sh

# controllo il numero di parametri
case $# in
	0|1|2) echo Errore numero parametri! Usage: $0 /////////////
		exit 1
		;;
	*) ;;
esac

