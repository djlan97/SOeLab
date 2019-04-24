#!/bin/sh
#File comandi ricorsivo

cd $1
#definiamo una variabile trovato
trovato=false

#controlliamo anche la radice della gerarchia
case $1 in
*/$2) 
#solo nel direttorio di nome relativo corretto cerchiamo i file con le specifiche richieste
	for i in *
	do
		if test -f $i 
		then
			#ridirigiamo standard output e standard error del grep su /dev/null
	  		if grep [0-9] $i > /dev/null 2>&1
	  		then 
			#salviamo il nome assoluto nel file temporaneo
	  		echo `pwd`/$i >> $3
			#settiamo trovato a true
			trovato=true
	  		fi
		fi
	done ;;
esac

if test $trovato = true
#se trovato uguale a true vuol dire che abbiamo trovato il direttorio DR che contiene almeno un file con un carattere numerico
then
  #quindi stampiamone il nome assoluto
  echo TROVATO DIRETTORIO che soddisfa le specifiche `pwd`
fi

#ricorsione in tutta la gerarchia
for i in *
do
if test -d $i -a -x $i
then
  $0 `pwd`/$i $2 $3 
fi
done
