/* Soluzione della Prova d'esame del 5 Giugno 2015 - SOLO Parte C */
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
	int M; 				/* numero di file passati sulla riga di comando (uguale al numero di file) */
	int status;			/* variabile di stato per la wait */
	pipe_t *piped;			/* array dinamico di pipe descriptors per comunicazioni figli-padre  */
	pipe_t p;			/* una sola pipe per ogni coppia figlio-nipote */ 
	int i, j;			/* indici per i cicli */
	char ch;			/* carattere per leggere dalla pipe collegata allo standard output del nipote (tail) e calcolare la lunghezza della ultima linea */
	int lunghezza; 			/* variabile che viene comunicata da ogni figlio al padre e che contiene la lunghezza dell'ultima linea */
	int ritorno; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene il ritorno del nipote */
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 3) /* Meno di due parametri */  
	{
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}

	/* Calcoliamo il numero di file passati */
	M = argc - 1;
	
	/* Allocazione dell'array di M pipe descriptors */
	piped = (pipe_t *) malloc (M*sizeof(pipe_t));
	if (piped == NULL)
	{
		printf("Errore nella allocazione della memoria\n");
		exit(2);
	}
	
	/* Creazione delle M pipe figli-padre */
	for (i=0; i < M; i++)
	{
		if(pipe(piped[i]) < 0)
		{
			printf("Errore nella creazione della pipe\n");
			exit(3);
		}
	}
		
	/* Ciclo di generazione dei figli */
	for (i=0; i < M; i++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork\n");
			exit(4);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio */
			printf("Sono il processo figlio di indice %d e pid %d sto per creare il nipote che recuperera' l'ultima linea del file %s\n", i, getpid(), argv[i+1]);
			/* in caso di errori nei figli o nei nipoti decidiamo di tornare dei numeri negativi (-1 che corrispondera' per il padre al valore 255, -2 che corrispondera' a 254, etc.) che non possono essere valori accettabili di ritorno dato che il comando tail torna 0 in caso di successo e 1 in caso di insuccesso */

			/* Chiusura delle pipe non usate nella comunicazione con il padre  */
			for (j=0; j < M; j++)
			{
				close(piped[j][0]);
				if (i != j) close(piped[j][1]);
			}

			/* prima creiamo la pipe di comunicazione fra nipote e figlio */
		  	if (pipe(p) < 0)
                	{	
                        	printf("Errore nella creazione della pipe fra figlio e nipote!\n");
                        	exit(-2);
                	}

			if ( (pid = fork()) < 0)
			{
				printf("Errore nella fork di creazione del nipote\n");
				exit(-3);
			}	
			if (pid == 0) 
			{
			/* codice del nipote */
			printf("Sono il processo nipote del figlio di indice %d e pid %d e sto per recuperare l'ultima linea del file %s\n", i, getpid(), argv[i+1]);
				/* chiusura della pipe rimasta aperta di comunicazione fra figlio-padre che il nipote non usa */
				close(piped[i][1]);
				/* Ridirezione dello standard input (si poteva anche non fare e passare il nome del file come ulteriore parametro della exec):  il file si trova usando l'indice i incrementato di 1 (cioe' per il primo processo i=0 il file e' argv[1]) */
				close(0);
				if (open(argv[i+1], O_RDONLY) < 0)
				{
                                printf("Errore nella open del file %s\n", argv[i+1]);
                                exit(-4);
                        	}
				/* ogni nipote deve simulare il piping dei comandi nei confronti del figlio e quindi deve chiudere lo standard output e quindi usare la dup sul lato di scrittura della propria pipe */
				close(1);
				dup(p[1]); 			
				/* ogni nipote adesso puo' chiudere entrambi i lati della pipe: il lato 0 non viene usato e il lato 1 viene usato tramite lo standard ouput */
				close(p[0]);
				close(p[1]);
				/* Ridirezione dello standard error su /dev/null (per evitare messaggi di errore a video)*/
				close(2);
				open("/dev/null", O_WRONLY);
			
				/* Il nipote diventa il comando tail -1 */				
				execlp("tail", "tail", "-1", (char *)0);
				/* attenzione ai parametri nella esecuzione di tail: aolo -1 e terminatore della lista. */ 
				
				/* Non si dovrebbe mai tornare qui!!: ATTENZIONE avendo chiuso lo standard output e lo standard error NON si possono fare stampe con indicazioni di errori; nel caso, NON chiudere lo standard error e usare perror o comunque write su 2 */
				exit(-5); /* torno un valore diverso da zero per indicare insuccesso (vedi altri commenti nel seguito) */
			}
			else { /* codice figlio */
	/* ogni figlio deve chiudere il lato che non usa della pipe di comunicazione con il nipote */
			close(p[1]);
	/* adesso il figlio legge dalla pipe */
			j=0;
		        while (read(p[0], &ch, 1))
			{
/*
				printf("indice j= %d carattere letto da pipe %c\n", j, ch);
*/
				j++;
			}
			if (j!=0) /*se il figlio ha letto qualcosa */
				 {
					lunghezza=j-1;
/* decrementando di 1 il valore di j otteniamo la lunghezza della linea escluso il terminatore; se teniamo invece j avremmo la lunghezza della linea compreso il terminatore */
/*
					printf("valore calcolato dal figlio %d\n", lunghezza);
*/
				 }
			else 
				{
                                        lunghezza=0;
/*
                                        printf("il nipote non ha passato alcuna linea di cui calcolare la lunghezza\n");
*/
                                 }

	/* il figlio comunica al padre */
			write(piped[i][1], &lunghezza, sizeof(lunghezza));

	/* il figlio deve aspettare il nipote per restituire il valore al padre */
	/* se il nipote e' terminato in modo anomalo decidiamo di tornare -1 che verra' interpretato come 255 e quindi segnalando questo problema al padre */
			ritorno=-1;
			pid = wait(&status);
			if (pid < 0)
			{	
				printf("Errore in wait\n");
				exit(-6);
			}
			if ((status & 0xFF) != 0)
    				printf("Nipote con pid %d terminato in modo anomalo\n", pid);
    			else
				printf("Il nipote con pid=%d ha ritornato %d\n", pid, ritorno=(int)((status >> 8) & 0xFF));
			exit(ritorno);
  			}	
		}
	}
	
/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < M; i++)
		close(piped[i][1]);

/* Il padre recupera le informazioni dai figli in ordine inverso di indice */
	for (i=M-1; i >= 0; i--)
	{
		/* il padre recupera tutti i valori interi dai figli */
		read(piped[i][0], &lunghezza, sizeof(lunghezza));
		printf("Il figlio di indice %d ha comunicato il valore %d per il file %s\n", i, lunghezza, argv[i+1]);
	}	

	/* Il padre aspetta i figli */
	for (i=0; i < M; i++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
		printf("Errore in wait\n");
		exit(5);
		}

		if ((status & 0xFF) != 0)
    		printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ ritorno=(int)((status >> 8) &	0xFF); 
		  if (ritorno==255)
 			printf("Il figlio con pid=%d ha ritornato %d e quindi vuole dire che il nipote non e' riuscito ad eseguire il tail oppure e' terminato in modo anormale\n", pid, ritorno);
		  else  printf("Il figlio con pid=%d ha ritornato %d\n", pid, ritorno);
		}
	}
exit(0);
}
