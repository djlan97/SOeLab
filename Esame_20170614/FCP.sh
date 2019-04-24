#!/bin/sh
#
# Soluzione dell'appello del 14 Giugno 2017
# File dei controlli
#
# $0 dirass D (dimensione in byte) C (carattere)
# controllo numero dei parametri
case $# in
3);;
*)
echo "Uso: $0 dirass D C..."
exit 1;;
esac
# controllo primo parametro direttorio assoluto
case $1 in
/*) 	if test ! -d $1 -o ! -x $1
	then
	echo "Direttorio $1 inesistente o inaccessibile"
	exit 2
	fi;;
*) 	echo "Nome $1 non assoluto"
	exit 3;;
esac

# controllo secondo parametro
case $2 in
*[!0-9]*) 	echo Secondo parametro $2 non numerico o non positivo
 		exit 4;;
*)		if test $2 -eq 0 
		then 	echo Secondo parametro $2 non strettamente maggiore di 0 
                	exit 5
		fi;;
esac
# controllo terzo parametro
case $3 in
?);;
*)
echo "Terzo parametro $3 non singolo carattere"
exit 6;;
esac

# impostazione di PATH
PATH=$PATH:`pwd`
export PATH

#creiamo file temporaneo
> /tmp/files$$

# invocazione della parte ricorsiva passando come ultimo parametro il nome del file temporaneo
FCR.sh $* /tmp/files$$

# contiamo le linee del file
FILES=`wc -l < /tmp/files$$`
#verifica e stampa non richiesta, ma inserita per maggior chiarezza
if test $FILES -gt 0
then
echo "Sono stati trovati globalmente $FILES file che rispettano le specifiche"
echo "che sono i seguenti"
cat /tmp/files$$
# invocazione della parte c
14Giu17 `cat /tmp/files$$` $3
else
echo "Non sono stati trovati file che soddisfano le specifiche"
fi

#cancelliamo il file temporaneo
rm /tmp/files$$
