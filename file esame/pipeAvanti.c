#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

typedef int pipe_t[2];
typedef struct{
        int id; 	/* indice figlio (campo c1 del testo) */
	int occ; 	/* numero occorrenze (campo c2 del testo) */
		} msg;

int mia_random(int n){
	int casuale;
	casuale = rand() % n;
	return casuale;
}


/* bubblesort che ordina l'array di strutture di tipo data passato (va ad ordinare in base ai valore del campo occ delle strutture facenti parte dell'array) */
void bubbleSort(data *v, int dim)
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


int main (int argc, char **argv){
	int N; 		/* numero di file */
	int H; 		/* numero di linee */
	int pid;	/* pid per fork */
	pipe_t *pipes;	/* array di pipe usate a pipeline da primo figlio, a secondo figlio .... ultimo figlio e poi a padre: ogni processo legge dalla pipe i-1 e scrive sulla pipe i */
	int i,j; 	/* contatori */
	int fd; 	/* file descriptor */
	int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
	char Cx;  	/* carattere da cercare */
	char linea[250];	/* supponiamo che bastino 250 caratteri per contenere ogni riga insieme con il terminatore di linea e con il terminatore di stringa */
	msg cur; 		/* struttura usata dal figlio corrente */
	int nr,nw;		/* variabili per salvare valori di ritorno di read e write da/su pipe */
	int stampate=0;		/* conta le linee stampate */

	if (argc < 4){
		printf("Errore numero di parametri\n");
		exit(1);
	}

	H = atoi(argv[argc-1]);
	printf("Numero di linee %d\n", H);
	if (H <= 0){
		printf("Errore numero linee: non strettamente maggiore di zero\n");
		exit(2); 
	}

	if (strlen(argv[argc-2]) != 1){
		printf("Errore penultimo parametro NON carattere\n");
		exit(3); 
	}

	Cx = argv[argc-2][0];

	N = argc-3;
	
	/* allocazione pipe usate in pipeline */
	if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL){
		printf("Errore allocazione pipe usate in pipeline\n");
		exit(4); 
	}
	/* creazione pipe */
	for (i=0;i<N;i++){
		if(pipe(pipes[i])<0){
			printf("Errore creazione pipe usate in pipeline\n");
			exit(6);
		}
	}

	/* creazione figli */
	for (i=0;i<N;i++){
		if ((pid=fork())<0){
			printf("Errore creazione figli\n");
			exit(7);
		}
		if(pid==0){
			/* codice figlio */
			printf("Sono il figlio %d\n", getpid());
			/* chiusura pipes inutilizzate */
			for (j=0;j<N;j++){
				if (j!=i){
					close(pipes[j][1]);
				}
				if ((i == 0) || (j != i-1)){
					close(pipes[j][0]);
				}
			}
	 
			/* apertura file */
			if ((fd=open(argv[i+1],O_RDONLY))<0){
				printf("Impossibile aprire il file %s\n", argv[i+1]);
				/* torniamo un valore 0 dato che non siamo stati in grado di aprire il file e quindi non sono state scritte linee da questo figlio */
				exit(0);
			}
			j = 0; /* indice per linea */
			while(read(fd,&linea[j],1)>0){
				
			}	
			exit(stampate);
		}
	} /* fine for */

	/* codice del padre */

	/* chiusura pipe non utilizzate */
	/* chiusura pipes: tutte meno l'ultima in lettura */
	for(i=0;i<N;i++){
		close(pipes[i][1]);
		if (i != N-1){ 
			close(pipes[i][0]);
		}
	}
	


	/* Il padre aspetta i figli */
	for (i=0; i < N; i++){
	        pidFiglio = wait(&status);
        	if (pidFiglio < 0){
        	        printf("Errore in wait\n");
        	        exit(7);
                }

        	if ((status & 0xFF) != 0){
                	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
		}else{ 
			ritorno=(int)((status >> 8) & 0xFF);
			printf("Il figlio con pid=%d ha ritornato %d (Se 255 problemi nel figlio!)\n", pidFiglio, ritorno);
                }
        }
	exit(0);
}
