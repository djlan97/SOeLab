/* Soluzione della Prova d'esame del 9 Febbraio 2015 - Parte C */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

typedef int pipe_t[2];

int main(int argc, char **argv) 
{
	/* -------- Variabili locali ---------- */
	int pid;			/* process identifier per le fork() */
	int N; 				/* numero di file passati sulla riga di comando (uguale al numero di numeri) */
	int status;			/* variabile di stato per la wait */
	pipe_t *piped;			/* array dinamico di pipe descriptors per comunicazioni figli-padre  */
	pipe_t p;			/* una sola pipe per ogni coppia figlio-nipote */ 
	int i, j;			/* indici per i cicli */
	char opzione[10];		/* variabile stringa che serve per passare al comando head l'opzione con il numero delle linee da stampare */
					/* supponiamo che bastino 8 cifre per il numero, oltre al simbolo di opzione '-' e al terminatore di stringa */
	char linea[250]; 		/* variabile che serve per leggere le linee sia da parte del figlio che del padre */
					/* supponiamo che bastino 250 caratteri per contenere ogni riga insieme con il terminatore di linea e con il terminatore di stringa */
	int lung; 			/* variabile che mantiene la lunghezza della linea mandata dal figlio al padre */
	int ritorno; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene il ritorno del nipote */
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 3 || (argc-1)%2) /* Meno di due parametri e non pari */  
	{
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}

	/* Calcoliamo il numero di file/numeri passati */
	N = (argc - 1)/2;
	
	/* Controlliamo i numeri passati: in questa soluzione non serve usare la funzione atoi() per trasformare queste stringhe in numeri */
	/* dato che il numero ci serve sotto forma di stringa dopo nella head. Quindi controlliamo il primo carattere di ogni stringa numerica */
	/* e se e' il carattere '-' vuol dire che il numero non e' strettamente positivo e diamo errore */
	for (i=0; i < N; i++)
	{
	/* le stringhe che rappresentano i numeri si trovano con l'indice i moltiplicato per 2 e sommando 2, cioe' nelle posizioni 2, 4, 6, etc. e quindi saltando le stringhe che rappresentano i nomi dei file */
		if (argv[(i*2)+2][0] == '-' )
		{
			printf("Errore %s non rappresenta un numero positivo \n", argv[(i*2)+2]);
			exit(2);
		}
	}
	/* Allocazione dell'array di N pipe descriptors */
	piped = (pipe_t *) malloc (N*sizeof(pipe_t));
	if (piped == NULL)
	{
		printf("Errore nella allocazione della memoria\n");
		exit(3);
	}
	
	/* Creazione delle N pipe figli-padre */
	for (i=0; i < N; i++)
	{
		if (pipe(piped[i]) < 0)
		{
			printf("Errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/* Le N pipe nipoti-figli deriveranno dalla creazione di una pipe in ognuno dei figli che poi creeranno un nipote */
		
	/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(5);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			/* Costruiamo la stringa di opzione per il comando head e che servira' al nipote: lo potrebbe fare anche il nipote invece che il figlio */
			/* Per prima cosa copiamo nella stringa vuota opzione la stringa con il simbolo di opzione */
			strcpy(opzione, "-");
			/* quindi concateniamo la stringa corrispondente al numero giusto per questa coppia figlio/nipote */
			strcat(opzione,argv[(i*2)+2]);
			/* stampa di controllo non richiesta */
			printf("Sono il processo figlio di indice %d e pid %d sto per creare il nipote che fara' la head del file %s con opzione %s\n", i, getpid(), argv[(i*2)+1], opzione);
			/* Chiusura delle pipe non usate nella comunicazione con il padre  */
			for (j=0; j < N; j++)
			{
				close(piped[j][0]);
				if (i != j) close(piped[j][1]);
			}

			/* prima creiamo la pipe di comunicazione fra nipote e figlio */
		  	if(pipe(p) < 0)
                	{	
                        	printf("Errore nella creazione della pipe\n");
                        	exit(6);
                	}

			if ( (pid = fork()) < 0)
			{
				printf("Errore nella fork di creazione del nipote\n");
				exit(7);
			}	
			if (pid == 0) 
			{
			/* codice del nipote */
			printf("Sono il processo nipote del figlio di indice %d e pid %d sto per calcolare la head con opzione %s per il file %s\n", i, getpid(), opzione, argv[(i*2)+1]);
				/* chiusura della pipe rimasta aperta di comunicazione fra figlio-padre che il nipote non usa */
				close(piped[i][1]);
				/* Ridirezione dello standard input:  il file si trova usando l'indice i moltiplicato per 2 e sommando 1, cioe' nelle posizioni 1, 3, 5, etc. e quindi saltando le stringhe che rappresentano i numeri delle linee */
				/* Questa ridirezione non e' strettamente necessaria: in alternativa bisogna passare il nome del file come parametro */
				close(0);
				/* le stringhe che rappresentano i nomi dei file si trovano con l'indice i moltiplicato per 2 e sommando 1, cioe' nelle posizioni 1, 3, 5, etc. e quindi saltando le stringhe che rappresentano i numeri*/
				if (open(argv[(i*2)+1], O_RDONLY) < 0)
				{
                                printf("Errore nella open del file %s\n", argv[(i*2)+1]);
                                exit(8);
                        	}
				/* ogni nipote deve simulare il piping dei comandi nei confronti del figlio e quindi deve chiudere lo standard output e quindi usare la dup sul lato di scrittura della propria pipe */
				close(1);
				dup(p[1]); 			
				/* ogni nipote adesso puo' chiudere entrambi i lati della pipe: il lato 0 NON viene usato e il lato 1 viene usato tramite lo standard ouput */
				close(p[0]);
				close(p[1]);
				/* Il nipote diventa il comando head: bisogna usare le versioni dell'exec con la p in fondo in modo da usare la variabile di ambiente PATH  */				
				execlp("head", "head", opzione, (char *)0);
				/* attenzione ai parametri nella esecuzione di head: aolo opzione e terminatore della lista dato che abbiamo usato la ridirezione in input, altrimenti si doveva passare il nome del file e non serviva aprirlo come invece fatto sopra */ 
				
				/* Non si dovrebbe mai tornare qui!!*/
				perror("Problemi di esecuzion della head da parte del nipote");
				exit(-1); /* torno un valore diverso da zero per indicare insuccesso (vedi altri commenti nel seguito) */
			}
			else { /* codice figlio */
				/* chiusura lato della pipe nipote-figlio non usato */
				close(p[1]);
				/* adesso il figlio legge dalla pipe fino a che ci sono caratteri e cioe' linee inviate dal nipote tramite la head */
				j=0; /* inizializziamo l'indice della linea */
		        	while (read(p[0], &(linea[j]), 1))
				{	
				/* se siamo arrivati alla fine di una linea */
				if (linea[j] == '\n')  
					/* si deve salvare il valore dell'indice prima di azzerarlo perche' non sappiamo se e' l'ultima linea (altrimenti si doveva controllare con il parametro numero tenendo un contatore delle linee) */
					{ lung=j; j=0; }
				else
					j++;
				}	
				/* quando si esce dal while di lettura, nell'array linea abbiamo l'ultima linea ricevuta: incrementiamo la sua lunghezza per inserire il terminatore di stringa che serve solo per poter usare le opzione di stampa delle stringhe, altrimenti si puo' non farlo */
				lung++;		
				linea[lung]='\0';
				/* il figlio comunica al padre: passiamo anche il terminatore di stringa per avere anche dal lato del padre il vantaggio di poter usare le opzioni di stampa delle stringhe */
				write(piped[i][1], linea, lung+1);

				/* il figlio deve aspettare il nipote per restituire il valore al padre */
				/* se il nipote e' terminato in modo anomalo decidiamo di tornare -1 che verra' interpretato come 255 e quindi segnalando questo problema al padre */
				ritorno=-1;
				pid = wait(&status);
				if (pid < 0)
				{	
					printf("Errore in wait\n");
					exit (9);
				}
				if ((status & 0xFF) != 0)
    					printf("Nipote con pid %d terminato in modo anomalo\n", pid);
    				else
					/* questa stampa non e' richiesta dal testo */
					printf("Il nipote con pid=%d ha ritornato %d\n", pid, ritorno=(int)((status >> 8) & 0xFF));
				exit(ritorno);
  				}	
		}
	}
	
/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < N; i++)
		close(piped[i][1]);

/* Il padre recupera le informazioni dai figli in ordine di indice */

	for (i=0; i < N; i++)
	{
		/* il padre recupera la linea spedita da ogni figlio leggendo da ogni pipe tutti i caratteri */
		j=0;
		while (read(piped[i][0], &(linea[j]), 1))
		{
			j++;
		}
		/* stampiamo le informazioni richieste dal testo. N.B. La linea puo' essere stampata tranquillamente dal padre con %s perchè e' una stringa C */
		printf("La linea\n%se' stata comunicata dal figlio %d-esimo: e' la linea numero %s del file %s\n", linea, i, argv[(i*2)+2], argv[(i*2)+1]);
	}	
	/* Il padre aspetta i figli */
	for (i=0; i < N; i++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
		printf("Errore in wait\n");
		exit (10);
		}

		if ((status & 0xFF) != 0)
    		printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ ritorno=(int)((status >> 8) &	0xFF); 
		  if (ritorno==255)
 			printf("Il figlio con pid=%d ha ritornato %d e quindi vuole dire che il nipote non e' riuscito ad eseguire la head oppure e' terminato in modo anormale\n", pid, ritorno);
		  else  printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}
	exit(0);
}
