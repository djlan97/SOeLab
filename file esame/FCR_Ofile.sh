#!/bin/sh

cd $1


#la variabile trovato ci serve per capire se il direttorio rispetta la specifica
trovato=false
NR= # numero linee di un file

for i in *
do
        #controlliamo solo i nomi dei file e se sono leggibili
        if test -f $i -a -r $i 
        then
                # calcoliamo la lunghezza in linee con il comando wc e la ridirezione in imput
		NR=`wc -l < $i`
                if test $NR -eq $2
                then
                	#abbiamo trovato un file che soddisfa le specifiche e quindi mettiamo a true trovato
                	trovato=true
			# salviamo il nome assoluto del file nel file temporaneo
			echo `pwd`/$i >> $3
                fi
        fi
done

#se abbiamo trovato almeno un file 
if test $trovato = true
then
	# stampo il nome assoluto su stdout
        echo Trovato direttorio `pwd` che soddisfa le specifiche
fi

for i in *
do
if test -d $i -a -x $i
then
  #invocazione ricorsiva
  $0 `pwd`/$i $2 $3
fi
done
