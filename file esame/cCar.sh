#controllo che il parametro sia un singolo carattere
case $4 in
?) echo carattere $4;;
*) echo $4 non singolo carattere
   exit 8;;
esac

