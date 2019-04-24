#!/bin/sh

# controllo sul numero dei parametri
case $# in
	3) ;;
	*) echo Errore numero parametri! Usage: $0 dirass K CZ
	exit 1
	;;
esac

# controllo che il primo parametro sia un nome assoluto di un direttorio traversabile
case $1 in
	/*) if test ! -d $1 -o ! -x $1
		then
				echo $1 non è un direttorio traversabile
				exit 2
		fi;;
	*) echo $1 non è un nome assoluto
		exit 3
		;;
esac

# controllo che il secondo parametro sia un numero intero strettamente positivo
case $2 in
	*[!0-9]*) echo $2 non è un numero oppure è un numero negativo
					exit 4
					;;
			*) if test $2 -eq 0
				then
						echo $2 non è strettamente positivo
						exit 5
				fi;;
esac

# controllo che il terzo parametro sia un singolo carattere
case $3 in
	?) ;;
	*) echo il terzo parametro $3 non è un singolo carattere
	exit 6
		;;
esac

# fine controllo parametri

#impostazione variabile PATH
PATH=`pwd`:$PATH
export PATH

# creo il file temporaneo per la chiamata alla parte c
> /tmp/file$$

# invocazione file ricorsivo
FCR.sh $* /tmp/file$$

# invocazione parte c
main `cat /tmp/file$$` $3  `expr $2 \ 2`

# infine eliminaimop il file temporaneo
rm /tmp/file$$
