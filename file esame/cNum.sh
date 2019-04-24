# controllo che il parametro sia un numero strettamente positivo e minore di 255
case $1 in
	*[!0-9]*) echo Primo parametro $1 non numerico o non positivo
                	exit 2;;
	*) if test $1 -eq 0 -o $1 -ge 255
	then    
		echo $1 non diverso da 0 oppure non minore di 255 
                        exit 3
	fi;;
esac

