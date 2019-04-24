#!/bin/sh

# $1 gerarchia (G)
# $2 numero intero positivo (K)
# $3 carattere (CZ)
# $4 file temporaneo

dim= # contiene la dimensione di un file della directory coarrente

# entro nel direttorio passato come primo parametro
cd $1

for i in *
do
	# controllo che sia un file
	if [ -f $i ]; then
			# devo usare la ridirezione in input con wc per ottenere la grandezza in byte del file
			dim=`wc -c < $i`
			if test $dim -eq $2
			then
				# se il file contiene il carattere CZ (passato come terzo parametro) stampo il nome assoluto del file su stdout e lo salvo nel file temporaneo
				if grep $3 $i > /dev/null 2>&1
				then
						# stampo su stdout
						echo TROVATO FILE `pwd`/$i

						# scrivo sul file
						echo `pwd`/$i >> $4
				fi
			fi
	fi
done


for i in *
do
	# se è un direttorio proseguo nella gerarchia
	if test -d $i -a -x $i 
	then
			# invocazione ricorsiva
			FCR.sh `pwd`/$i $2 $3 $4
	fi
done
