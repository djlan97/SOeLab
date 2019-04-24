#!/bin/sh
#Soluzione della Prova del 15 Febbraio 2017
#file comandi principale da invocare con dirass X

case $# in
2) ;;
*) 	echo Errore: Usage is $0 dirass X
	exit 1;;
esac

#Controllo primo parametro
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    echo $1 non direttorio
    exit 2
    fi;;
*)  echo $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro
expr $2 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then echo numerico $2 #siamo sicuri che numerico
     if test $2 -le 0 
     then echo $2 non strettamente positivo
       	  exit 4
     fi
else
  echo $2 non numerico
  exit 5
fi

PATH=`pwd`:$PATH
export PATH

#invocazione del file comando ricorsivo
FCR.sh $* 
