#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef int pipe_t[2];

int mia_random(int n){
	int casuale;
	casuale=rand()%n;
	return casuale;
}

int main(int argc, char **argv){

	int H, N;/* Contiene il numero di linee di ogni file */
	int pid;
	int pidFiglio, status;
	int fd,fdout;
	int i,j;
	pipe_t *pipedFP, *pipedPF;
	char linea[255];
	int valore;
	int giusto;
	int r;
	int ritorno=0;

	/* controllo numero parametri */
	if(argc<6){
		printf("ERRORE: errore numero parametri!\n");
		exit(1);
	}

	H=atoi(argv[argc-1]);
	if(H<=0 || H>=255){
		printf("ERRORE: %d non è un numero strettamente maggiore di zero e minore di 255!\n",H);
		exit(2);
	}

	if((fdout=open("/tmp/creato", O_CREAT|O_WRONLY|O_TRUNC,0644))<0){
		printf("ERRORE: errore creazione file %s\n","/tmp/creato");
		exit(3);
	}
	N=argc-2;
	pipedFP=(pipe_t *)malloc(N*sizeof(pipe_t));
	pipedPF=(pipe_t *)malloc(N*sizeof(pipe_t));
	

	/* allocazione memoria per le pipe */
	if(pipedFP==NULL || pipedPF==NULL){
		printf("ERRORE: errore allocazione di memoria per le pipe!\n");
		exit(4);
	}
	
	/* creazione pipe */
	for(i=0;i<N;i++){
		if(pipe(pipedFP[i])<0){
			printf("ERORE: errore creazione pipe figlio-padre!\n");
			exit(5);
		}

		if(pipe(pipedPF[i])<0){
			printf("ERRORE: errore creazione pipe padre-fidglio!\n");
			exit(6);
		}
	}
	
	printf("Sono il padre con PID=%d...\n", getpid());

	/* creazione figlio */
	for(i=0;i<N;i++){
		if((pid=fork())<0){
			printf("ERRORE: errore fork()!\n");
			exit(7);
		}
		if(pid==0){
			/* codice figlio */
			printf("Sono il processo figlio con PID=%d...\n",getpid());
			for(j=0;j<N;j++){
				close(pipedFP[j][0]);
				close(pipedPF[j][1]);
				if(i!=j){
					close(pipedFP[j][0]);
					close(pipedPF[j][1]);
				}
			}
			if((fd=open(argv[i+1],O_RDONLY))<0){
				printf("ERRORE: errore apertura file %s!\n",argv[i+1]);
				exit(-1);
			}

			j=0;
			while(read(fd,&(linea[j]),1)){

				j++;
				if(linea[j]=='\n'){
					write(pipedFP[i][1],&j,sizeof(j));
					read(pipedPF[i][0],&r,sizeof(r));
					if(r<j){
						write(fdout,&(linea[r]),1);
						ritorno++;

					}else{
						j=0;
					}
				}
			}
			exit(ritorno);
		}
	}


	/* codice padre */
	
	/* chiudo i lati delle pipe che non uso */
	for(i=0;i<N;i++){
		close(pipedFP[i][1]);
		close(pipedPF[i][0]);
	}

	for(j=1;j<=H;j++){
		r=mia_random(N);
		printf("Il numero generato in modo random (per la linea %d) compreso tra 0 e %d per selezionare il figlio è %d\n",j,N-1,r);
		for(i=0;i<N;i++){
			read(pipedFP[i][0],&valore,sizeof(valore));
			if(i==r){
				giusto=valore;
			}
		}

		r=mia_random(giusto);
		printf("Il numero generato in modo random compreso fra 0 e %d per selezionare l'indice della linea %d-iesima è %d\n",giusto-1,j,r);

		for(i=0;i<N;i++){
			write(pipedPF[i][1],&r,sizeof(r));
		}


	}

	/* il padre aspetta i figli */

	for(i=0;i<N;i++){
		if((pidFiglio=wait(&status))<0){
			printf("ERRORE: errore wait()!\n");
			exit(8);
		}
		if((status & 0xff)!=0){
			printf("Figlio con PID=%d terminato in modo anomalo!\n",pidFiglio);
		}else{
			ritorno=(int)((status>>8) & 0xFF);
			printf("Figlio con PID=%d ha ritornato %d (se 255 errori nl figlio!)\n",pidFiglio,ritorno);
		}
	}
	exit(0);
}
