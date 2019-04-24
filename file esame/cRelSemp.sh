#Controllo che il parametro sia un nome relativo semplice
case $2 in
	*/*) echo $2 NON relativo semplice
     		exit 4;;
esac


