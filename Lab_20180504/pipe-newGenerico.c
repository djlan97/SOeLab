/* FILE: pipe-new.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h> 

int main (int argc, char **argv)
{  	int pid, j, piped[2],L;
	int pidFiglio,status; 
	char mess[512];
	char inpbuf [512];
	int Lun;	
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
close (piped [0]); 	/* il figlio CHIUDE il lato di lettura */
if ((fd = open(argv[1], O_RDONLY)) < 0)
{   printf("Errore in apertura file %s\n", argv[1]);
    exit(4);
}

printf("Figlio %d sta per iniziare a scrivere una serie di messaggi sulla pipe dopo averli letti dal file passato come parametro\n", getpid());
L=0;
j=0; /* il figlio inizializza la sua variabile j per contare i messaggi che ha mandato al padre */
while (read(fd, &mess[L], 1)!=0)
{   
	if(mess[L]=='\n')
	{/* il padre ha concordato con il figlio che gli mandera' solo stringhe e quindi dobbiamo sostituire il terminatore di linea con il terminatore di stringa */
		mess[L]='\0'; 
		L++;
		write(piped[1],&L,sizeof(L));
		write (piped[1], mess, L);
		L=0;
		j++;
	}else L++;
}
printf("Figlio %d scritto %d messaggi sulla pipe\n", getpid(), j);
exit (0);
}

else  
{   
/* padre */
close (piped [1]); /* il padre CHIUDE il lato di scrittura */		
printf("Padre %d sta per iniziare a leggere i messaggi dalla pipe\n", getpid());
j=0; /* il padre inizializza la sua variabile j per verificare quanti messaggi ha mandato il figlio */
while (read ( piped[0], &Lun, sizeof(Lun)))
    { 	
	read(piped[0],inpbuf,Lun);
        /* letto la lunghezza */
        printf ("%d: %s\n", j,inpbuf);
        j++;
     }
printf("Padre %d letto %d messaggi dalla pipe\n", getpid(), j);
pidFiglio=wait ((int *)0); /* padre aspetta il figlio */
if(pidFiglio<0)
		{
			printf("ERRORE: errore wait!\n");
			exit(4);
		}
		if((status & 0xFF)!=0)
		{
			printf("Figlio terminato in modo anomalo!\n");
		}else{
			printf("Il figlio con PID=%d ha ritornato %d\n",pidFiglio,(int)((status>>8)&0xFF));
		}



exit (0);
}
}
