#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	int n;/* Contiene il numero di figli passato come primo parametro */
	int i;/* Contatore per l'iterazione */
	int pid;/* Per la fork() */
	int status;/* Per la wait */
	int pidFiglio;/* Per la wait */

	if(argc!=2)
	{
		printf("ERRORE: il programma %s deve essere invocato con esattamente 1 parametro!\n",argv[0]);
		exit(1);
	}

	n=atoi(argv[1]);

	if(n<=0 || n>=255)
	{
		printf("ERRORE: il parametro deve essere un numero intero strettamente maggiore di 0 e minore di 255!\n");
		exit(2);
	}
	/* Creazione figli */
	for(i=0;i<n;i++){
		if((pid=fork())<0)
		{
			printf("ERRORE: errore fork()!\n");
			exit(3);
		}
		if(pid==0)
		{
			printf("Sono il figlio con PID=%d di indice %d\n",getpid(),i);
			exit(i);
		}

	}

	for(i=0;i<n;i++)
	{
		pidFiglio=wait(&status);
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
	}

	exit(0);
}
