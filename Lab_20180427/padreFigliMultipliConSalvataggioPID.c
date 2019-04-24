#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <time.h>

int mia_random(int n)
{
	int casuale;
	casuale=rand()%n;
	return casuale;
}

int main(int argc, char **argv)
{
	int n;/* Contiene il numero di figli passato come primo parametro */
	int i;/* Contatore per l'iterazione */
	int status;/* Per la wait */
	int pidFiglio;/* Per la wait */
	int nr;/* numero casuale */
	int *pid=NULL;/* vettore dinamico contenente i pid dei figi */
	int j;/* contatore per il ciclo di ricerca del pid */

	if(argc!=2)
	{
		printf("ERRORE: il programma %s deve essere invocato con esattamente 1 parametro!\n",argv[0]);
		exit(1);
	}

	n=atoi(argv[1]);

	if(n<=0 || n>=155)
	{
		printf("ERRORE: il parametro deve essere un numero intero strettamente maggiore di 0 e minore di 155!\n");
		exit(2);
	}


	if((pid=malloc(n*sizeof(int)))==NULL)
	{
		printf("ERRORE: errore malloc()!\n");
		exit(3);
	}


	/* Creazione figli */
	for(i=0;i<n;i++){
		if((pid[i]=fork())<0)
		{
			printf("ERRORE: errore fork()!\n");
			exit(4);
		}
		if(pid[i]==0)
		{
			printf("Sono il figlio con PID=%d di indice %d\n",getpid(),i);
			pid[i]=getpid();
			/* calcolo un numero casuale */
			nr=mia_random(100+i);
			exit(nr);
		}

	}

	for(i=0;i<n;i++)
	{
		pidFiglio=wait(&status);
		if(pidFiglio<0)
		{
			printf("ERRORE: errore wait!\n");
			exit(5);
		}
		if((status & 0xFF)!=0)
		{
			printf("Figlio terminato in modo anomalo!\n");
		}else{

			for(j=0;j<n;j++)
			{
				if(pidFiglio==pid[j])
				{
					printf("Il figlio di indice %d  con PID=%d ha ritornato %d\n",i,pidFiglio,(int)((status>>8)&0xFF));
					break;
				}
			}
		}
	}

	exit(0);
}
