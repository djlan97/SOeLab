#!/bin/sh

cd $1
#definiamo una variabile trovato
trovato=false

#controlliamo anche la radice della gerarchia
case $1 in
*/$2) 
#solo nel direttorio di nome relativo corretto cerchiamo i file con le specifiche richieste
	for i in *
	do
	
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
