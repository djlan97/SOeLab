#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int finito;/* per leggere le pipe dei figli */
	int ret;/* conta le occorrenze trovate dal figlio */
	int continua;/* per dire al figlio se continuare a leggere */
	char ch;/* per la read */
	long int pos;/* per inviare la posizione al padre */
	int pid;/* per la fork */
	pipe_t *pipedPF;/* per la pipe di comunicazione tra padre e figlio */
	pipe_t *pipedFP;/* per la pipe di comunoicazione tra figlio e padre */
	int pidFiglio,status;/* per la wait */
	int fd;/* per la open */
	int i,j;/* indici cicli */
	int N;/* numero di file passati come parametro */
	char CZ;/* contiene il penultimo parametro */
	int Y;/* contiene l'ultimo parametro */
	
	/* per avere senso devon avere almeno un file */
	if(argc<4){
		printf("ERRORE: il file deve essere invocato con almeno 3 parametri!\n");
		exit(1);
	}
	/* salvo il numero di fuile passati come parametro */
	N=argc-3;
	
	/* controllo che il secondo parametro sia un singolo carattere */
	if(strlen(argv[argc-2])!=1)
	{
		printf("ERRORE: il secondo parametro deve essere un singolo carattere!\n");
		exit(2);
	}

	/* salvo il penultimo carattere */
	CZ=argv[argc-2][0];
	/* controllo che l'ultimo parametro sia un numero strettamnete maggiore di 0 */
	Y=atoi(argv[argc-1]);
	if(Y<=0){
		printf("ERRORE: l'ultimo parametro deve essere un numero strettamente maggiore di 0!\n");
		exit(3);
	}

	/* alloco la memoria per le pipe */
	pipedFP=(pipe_t *)malloc(N*sizeof(pipe_t));
	pipedPF=(pipe_t *)malloc(N*sizeof(pipe_t));
	if((pipedFP==NULL)||(pipedPF==NULL)){
		printf("ERRORE: errore allocazione memoria!\n");
		exit(4);
	}

	/* creo le pipe */
	for(i=0;i<N;i++){
		if(pipe(pipedPF[i])<0){
			printf("ERRORE: errore creazione pipe tra padre e figlio!\n");
			exit(5);

		}
		if(pipe(pipedFP[i])<0){
			printf("ERRORE: errore creazione pipe tra figlio e padre!\n");
			exit(6);
		}
	}
	printf("Sono il padre con pid %d...\n",getpid());

	/* ciclo creazione figli */
	for(i=0;i<N;i++){
		if((pid=fork())<0){
			printf("ERRORE: errore fork\n");
			exit(7);
		}
		if(pid==0){
			/* codice figlio */
			printf("Sono il figlio con pid %d...\n",getpid());

			/* chiudo i lati delle pipe che non utilizzo */
			for(j=0;j<N;j++){
				close(pipedFP[j][0]);
				close(pipedPF[j][1]);
				if(i!=j){
					close(pipedPF[j][0]);
					close(pipedFP[j][1]);
				}
			}

			/* apro il file associato al figlio */
			if((fd=open(argv[i+1],O_RDONLY))<0){
				printf("ERRORE: impossibile aprire il file %s!\n",argv[i+1]);
				exit(-1);
			}

			/* leggo il file */
			ret=0;
			pos=(long int)0;
			while(read(fd,&ch,1)){
				if(ch==CZ){
					ret++;
					write(pipedFP[i][1],&pos,sizeof(pos));
					read(pipedPF[i][0],&continua,sizeof(continua));
					if(!continua){
						break;
					}
				}


			}
			exit(ret);
		
		}


	}

	/* codice padre */

	/* chiudo le i lati delle pipe che non utilizzo */
	for(i=0;i<N;i++){
		close(pipedPF[i][0]);
		close(pipedFP[i][1]);
	}

	/* leggo i caratteri inviati dalle pipe */
	finito=0;
	while(!finito){
		finito=1;
		for(i=0;i<N;i++){
			if(read(pipedFP[i][0],&pos,sizeof(pos))>0){	
				/* se pos è minore di Y può continuare (1) altrimenti no (0) */
				if(pos<Y){
					continua=1;
				}else{
					continua=0;
				}
				write(pipedPF[i][1],&continua,sizeof(continua));
			}
		}
	}

	/* il padre aspetta i figli */
	for(i=0;i<N;i++){
		if((pidFiglio=wait(&status))<0){
			printf("ERRORE: errore wait\n");
			exit(8);
		}
		if(WIFEXITED(status)){
			/* figlio terminato normalmente */
			printf("Figlio con pid %d terminato con exit %d\n",pidFiglio,WEXITSTATUS(status));
		}else{
			/* figlio terminato in modo anomalo */
			printf("Figlio con pid %d terminato in modo anomalo!\n",pidFiglio);	
		}
	}
	exit(0);
}
