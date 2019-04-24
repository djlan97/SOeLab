L=$1 #salviamo il primo parametro
#quindi ora possiamo usare il comando shift
shift
#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sui direttori in un for
for i
do
        case $i in
        /*) if test ! -d $i -o ! -x $i
            then
            	echo $i non direttorio
            exit 4
            fi;;
        *)  echo $i non nome assoluto; 
		exit 5;;
        esac
done

