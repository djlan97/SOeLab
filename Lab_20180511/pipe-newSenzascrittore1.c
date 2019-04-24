/* FILE: pipe-newSenzascrittore.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#define MSGSIZE 5 

int main (int argc, char **argv)
{  	int pid, j, piped[2]; 
	/* char mess[MSGSIZE]; */
	char inpbuf [MSGSIZE];                                    	 
if (argc != 2)
{   printf("Numero dei parametri errato\n");
    exit(1);
}
/* si crea una pipe */
if (pipe (piped) < 0 )  { exit (2); }
if ((pid = fork()) < 0)  { exit (3); }
else if (pid == 0)  
{   
/* figlio */
int fd;
//close (piped [0]); 	/* il figlio CHIUDE il lato di lettura */
if ((fd = open(argv[1], O_RDONLY)) < 0)
{   printf("Errore in apertura file %s\n", argv[1]);
    exit(4);
}

printf("Figlio %d sta per iniziare a scrivere una serie di messaggi, ognuno di lunghezza %d, sulla pipe dopo averli letti dal file passato come parametro\n", getpid(), MSGSIZE);
/* IL FIGLIO TERMINA ===> PIPE SENZA SCRITTORE */
exit (0);
}

else  
{   
/* padre */
//close (piped [1]); /* il padre CHIUDE il lato di scrittura */		
printf("Padre %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());
j =0; /* il padre inizializza la sua variabile j per verificare quanti messaggi ha mandato il figlio*/
while (read ( piped[0], inpbuf, MSGSIZE))
    { 	
        /* dato che il figlio gli ha inviato delle stringhe, il padre le puo' scrivere direttamente con una printf */
        printf ("%d: %s\n", j, inpbuf);
        j++;
     }
if (j != 0) 
	printf("Padre %d letto %d messaggi dalla pipe\n", getpid(), j);
else { puts("NON C'E' SCRITTORE"); exit(4); }
wait ((int *)0); /* padre aspetta il figlio */	
exit (0);
}
}
