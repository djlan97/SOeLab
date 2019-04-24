#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

typedef int pipe_t[2];

typedef struct{
	int num;
	char c;

}msg;

int mia_random(int n){
	int casuale;
	casuale = rand() % n;
	return casuale;
}


/* bubblesort che ordina l'array di strutture di tipo data passato (va ad ordinare in base ai valore del campo occ delle strutture facenti parte dell'array) */
void bubbleSort(msg *v, int dim)
{
	int i;
	data tmp;
	int ordinato = 0;
	while(dim > 0 && !ordinato)
	{
		ordinato = 1;
		for(i=0; i < dim-1; i++)
		{
			if(v[i].occ > v[i+1].occ)
			{
				tmp = v[i];
				v[i] = v[i+1];
				v[i+1] = tmp;
				ordinato = 0;
			}
		}
	
		dim--;
	}

}

int main(int argc, char **argv){
	/* -------- Variabili locali ---------- */
	int pid;			/* process identifier per le fork() */
	int N; 				/* numero di file passati sulla riga di comando (uguale al numero di file) */
	int H; 				/* numero passato come ultimo parametro e che rappresenta la lunghezza in linee dei file passati sulla riga di comando */
	msg strut;			/* struttura per la comunicazione dei dati */
	char ch;			/* carattere per la read */
	char Cx; 		 	/* carattere da cercare */
	int fd; 			/* file descriptor per apertura file */
	int pidFiglio, status;			/* valore di ritorno e variabile di stato per la wait */
	pipe_t *p;		/* array dinamico di pipe descriptors per comunicazioni figli-padre */
	int i, j;			/* indici per i cicli */
	int ritorno=0; 			/* variabile che viene ritornata da ogni figlio al padre e che contiene il numero di caratteri scritti sul file (supposta minore di 255) */
	/* ------------------------------------ */
	
	/* Controllo sul numero di parametri */
	if (argc < 4){ /* Meno di cinque parametri */ 
		printf("Errore nel numero dei parametri\n");
		exit(1);
	}
	printf("Sono il padre con pid %d\n", getpid());
	srand(time(NULL)); /* inizializziamo il seme per la generazione random di numeri  */

	/* Calcoliamo il numero di file passati */
	N = argc - 2;

	/* controlliamo che il penultimo parametro sia un carattere */
	if (strlen(argv[argc-2]) != 1){
		printf("Errore penultimo parametro NON carattere\n");
		exit(3); 
	}
	
	/* salviamo il carattere in Cx */
	Cx = argv[argc-2][0];

	/* convertiamo l'ultima stringa in un numero */
	H = atoi(argv[argc-1]);
	/* controlliamo il numero H: deve essere strettamente positivo e minore (o uguale) a 255 */
	if ((H <= 0) || (H >= 255)){
		printf("Errore nel numero passato %d\n", H);
		exit(2);
	}

	/* Allocazione dei due array di N pipe descriptors*/
	p = (pipe_t *) malloc (N*sizeof(pipe_t));
	if (p == NULL){
		printf("Errore nella allocazione della memoria\n");
		exit(3);
	}
	
	/* Creazione delle N pipe figli-padre e delle N pipe padre-figli */
	for (i=0; i < N; i++){
		if(pipe(p[i]) < 0){
			printf("Errore nella creazione della pipe figli-padre\n");
			exit(4);
		}
		
	}
	
/* Ciclo di generazione dei figli */
	for (i=0; i < N; i++){
		if ( (pid = fork()) < 0){
			printf("Errore nella fork\n");
			exit(6);
		}
		
		if (pid == 0){
			/* codice del figlio */
			printf("Sono il processo figlio di indice %d e pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);
			/* Chiusura delle pipe non usate nella comunicazione con il padre */
			for (j=0; j < N; j++){
				close(p[j][0]);
				if (i != j){
					close(p[j][1]);
				}
			}
			/* apertura del file associato in sola lettura */
			if ((fd=open(argv[i+1], O_RDONLY)) < 0){
                                printf("Errore nella open del file %s\n", argv[i+1]);
                                exit(-1); /* decidiamo di tornare -1 che verra' interpretato dal padre come 255 e quindi un valore non ammissibile! */
                       	}
			/* adesso il figlio legge dal file una linea alla volta */
			j=0;
		        while (read(fd, &ch, 1)){
					/* codice */




			}
			/* il figlio Pi deve ritornare al padre il valore corrispondente al numero di caratteri scritti sul file */
			exit(ritorno);
  		}	
	}
	
/* Codice del padre */
/* Il padre chiude i lati delle pipe che non usa */
	for (i=0; i < N; i++){
		close(p[i][1]);
        }


	/* codice */





	/* Il padre aspetta i figli */
	for (i=0; i < N; i++){
		pidFiglio = wait(&status);
		if (pidFiglio < 0){
		printf("Errore in wait\n");
		exit(7);
		}

		if ((status & 0xFF) != 0){
    			printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
    		}else{ ritorno=(int)((status >> 8) &	0xFF); 
		  printf("Il figlio con pid=%d ha ritornato %d (Se 255 problemi nel figlio!)\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}
