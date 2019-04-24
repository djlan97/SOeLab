#!/bin/sh

cd $1


#la variabile trovato ci serve per capire se tutti i file rispettano la specifica
trovato=true
#la variabile files serve per salvare i nomi assoluti dei file che saranno inseriti nel file temporaneo solo se il direttorio contiene solo file che rispettano le specifiche
files=

for i in *
do
        #controlliamo solo i nomi dei file 
        if test -f $i
        then
                #controlliamo che contenga solo caratteri alfabetici minuscoli
                if grep -v [a-z] $i > /dev/null
                then
                #abbiamo trovato un file che NON soddisfa le specifiche e quindi mettiamo a false trovato
                trovato=false
                else
                #abbiamo trovato un file che soddisfa le specifiche e quindi mettiamo il nome assoluto nella variabile files
                files="$files `pwd`/$i"
                fi
        fi
done
#se il booleano e' rimasto al valore iniziale e se abbiamo trovato almeno un file allora abbiamo trovato un direttorio giusto
if test $trovato = true -a "$files"
then
        echo Trovato direttorio `pwd` che soddisfa le specifiche
#inseriamo quindi i nomi assoluti dei file accumulati in files nel file temporaneo
        echo $files >> $2
fi

for i in *
do
if test -d $i -a -x $i
then
  #invocazione ricorsiva
  $0 `pwd`/$i $2 $3 $4
fi
done
