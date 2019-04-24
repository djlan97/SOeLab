#!/bin/sh

if test $# -ne 1
then
	echo "usage: $0 arg"
	exit 1
fi

case $1 in
	/*) echo "nome assoluto" ;;
	*/*) echo "nome relativo" ;;
	*) echo "nome relativo semplice" ;;
esac

exit 0
