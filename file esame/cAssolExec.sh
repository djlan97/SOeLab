# controllo che il parametro sia il nome assoluto di un direttorio
case $1 in
	/*) if test ! -d $1 -o ! -x $1
	then
    		echo $1 non direttorio
	    	exit 2
    	fi;;
	*) echo $1 non nome assoluto
    		exit 3;;
esac

