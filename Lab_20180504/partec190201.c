#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

int main(int argc, char **argv){
	int fd;/* per la open */
	int N;/* contiene il numero di file passati */
	int i;/* contatore ciclo */
	int pp[2];/* pipe file pari */
	int pd[2];/* pipe file dispari */
	int pid;/* per la fork */
	int pidFiglio,status;/* per la wait */
	char c;/* per leggere i caratteri dei file */
	long int tot;/* totale caratteri scritti dal padre */
	char cn;/* carattere numerico */
	char ca;/* carattere alfabetico */
	int nn;/* per read pipe 'dispari' */
	int na;/* per read pipe 'pari' */
	int ret;

	/* perchè l'esercizio abbia senso sono necessari almeno 2 parametri */
	if(argc<3){
		printf("ERRORE: necessari almeno 2 parametri!\n");
		exit(1);
	}
	
	/* creo le due pipe */
	if((pipe(pp))<0){
		printf("ERRORE: errore pipe()!\n");
		exit(2);
	}

	if((pipe(pd))<0){
		printf("ERRORE: errore pipe()!\n");
		exit(3);
	}

	N=argc-1;

	for(i=0;i<N;i++){
		if((pid=fork())<0){
			printf("ERRORE: errore fork()!\n");
			exit(4);		
		}
		/* figlio */
		if(pid==0){
			/* chiudo i lati di lettura delle due pipe */
			close(pd[0]);
			close(pp[0]);
			if((fd=open(argv[i+1],O_RDONLY))<0){
				printf("ERRORE: errore apertura file %s, file inesistente!\n",argv[i+1]);
				exit(5);
			}
			
			if((i+1)%2==0){
				/* pari */
				close(pd[1]);/* chiudo il lato di scrittura della pipe 'dispari' */

				while(read(fd,&c,1)>0){
					if(isdigit(c)){
						write(pp[1],&c,1);
					}
				}

				exit(0);
			
			}else{
				/*dispari */
				close(pp[1]);/* chiudo il lato di scrittura della pipe 'pari' */

				while(read(fd,&c,1)>0){
					if(isalpha(c)){
						write(pd[1], &c,1);
					}
				}
				exit(0);
			}
		}
	}/* for */


	/* padre */

	/* chiudo i lati di scrittura */
	close(pp[1]);
	close(pd[1]);
	tot=0;
	nn=read(pp[0],&cn,1);
	na=read(pd[0],&ca,1);
	while(nn || na){
		
		tot+=na;
		tot+=nn;

		write(1,&ca,na);
		write(1,&cn,nn);

		nn=read(pp[0],&cn,1);
		na=read(pd[0],&ca,1);

	}

	printf("\nIl padre Ha scritto %ld caratteri.\n",tot);

	for(i=0;i<N;i++){
		if((pidFiglio=wait(&status))<0){
			printf("ERRORE: errore wait()!\n");
			exit(6);
		}

		if((status & 0xFF)!=0){
			printf("ATTENZIONE: fglio con PID=%d terminato in modo anomalo!\n",pidFiglio);
		}else{
			ret=(int)((status>>8) & 0xFF);
			printf("Il figlio con PID=%d ha ritornato %d.\n",pidFiglio, ret);
		}
	}
	exit(0);

}/* main */
