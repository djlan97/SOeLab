#!/bin/sh
#file comandi ricorsivo dirass D C filetemp

cd $1

#per prima cosa azzeriamo il conteggio dei file che soddisfano la specifica 
contafile=0

for i in *
do
	if test -f $i -a -r $i -a -w $i #si deve cercare un file leggibile e SCRIVIBILE!
	then
	  	#se e' un file leggibile e scrivibile controlliamo la sua lunghezza e se contiene il carattere che dobbiamo cercare
		if test `wc -c < $i` -eq $2 
		then 	if grep $3 $i > /dev/null 2> /dev/null
			then
				echo `pwd`/$i >> $4  #salviamo il nome nel file temporaneo
				contafile=`expr $contafile + 1`	#incrementiamo il numero di file trovati
			fi
		fi
	fi	
done

#se ho trovato almeno un file 
if test $contafile -ge 1
then
  echo TROVATO DIRETTORIO `pwd` #riportiamo su standard output il nome assoluto
fi

for i in *
do
if test -d $i -a -x $i
then
  #echo RICORSIONE in `pwd`/$i 
  FCR.sh `pwd`/$i $2 $3 $4	#come primo parametro passiamo il NOME ASSOLUTO del direttorio (come nella prima invocazione!)
fi
done
