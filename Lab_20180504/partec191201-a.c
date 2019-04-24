#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
typedef int pipe_t[2];

int main(int argc, char **argv){
	int nc;/* numero caratteri pasati come parametro */
	int i,j;/* contatore ciclo */
	int pid;/* per la fork */
	int fd;/* per la open */
	long int count;/* per contare le occorrenze*/
	int pidFiglio,status;/* per la wait */
	pipe_t *piped;/* vettore di pipe */
	char c;/* la read */
	int ret;


	if(argc<3){
		printf("ERRORE: il file %s deve essere invocato con almeno 2 parametri!\n",argv[0]);
		exit(1);
	}

	nc=argc-2;

	for(i=0;i<nc;i++){
		if(strlen(argv[i+2])!=1){
			printf("ERRORE: il parametro #%d non è un carattere!\n",i+2);
			exit(2);
		}
	}


	/* alloco lo spazio per le pipe */

	piped=(pipe_t *)malloc(nc*sizeof(pipe_t));
	if(piped==NULL){
		printf("ERRORE: errore malloc()!\n");
		exit(3);
	}

	for(i=0;i<nc;i++){
		if((pipe(piped[i]))<0){
			printf("ERRORE: errore creazione pipe!\n");
			exit(4);
		}
	}

	printf("Sono il padre con PID=%d...\n",getpid());
	
	for(i=0;i<nc;i++){
		if((pid=fork())<0){
			printf("ERRORE: errore fork()!\n");
			exit(5);
		}

		if(pid==0){
			/* figlio */
			printf("Sono il figlio con PID=%d...\n",getpid());
			/* chiudiamo la parte di lettura di tutte le pipe e quelle di scrittura tranne la i-esima */
			for(j=0;j<nc;j++){
				close(piped[j][0]);
				if(j!=i){
					close(piped[j][1]);	
				}
			}

			/* apriamo il file */

			if((fd=open(argv[1],O_RDONLY))<0){
				printf("ERRORE: errore apertura file %s!\n",argv[1]);
				exit(6);			
			}
			count=0;
			while(read(fd,&c,1)!=0){
				if(c==argv[i+2][0]){
					count++;
				}
			}

			write(piped[i][1],&count,sizeof(count));

			exit(0);		
		}
	}/* for creazione figli */
	
	/* chiudo i lati di scrittura di tutte le pipe */

	for(i=0;i<nc;i++){
		close(piped[i][1]);
	}

	
	for(i=0;i<nc;i++){
		if(read(piped[i][0],&count,sizeof(count))>0){
			printf("Le occorreze nel file %s del carattere %c sono %ld.\n",argv[1],argv[i+2][0],count);
		}
	}


	/* aspetto i figli */

	for(i=0;i<nc;i++){
		if((pidFiglio=wait(&status))<0){
			printf("ERRORE: errore wait()!\n");
			exit(7);
		}

		if((status & 0xFF)!=0){
			printf("ATTENZIONE: figlio con PID=%d terminato in modo anomalo!\n",pidFiglio);
		}else{
			ret=(int)((status >> 8) & 0xFF);
			printf("Il figlio con PID=%d ha ritornato %d.\n",pidFiglio,ret);
		}

	}



	

	exit(0);
}
