#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

int contaOccorrenze(int fd,char Cx)
{
	int conta=0;
	char c;
	while(read(fd,&c,1)!=0)
	{
		if(c==Cx) conta++;
	}

	return conta;
}

int main(int argc, char **argv)
{
	char Cx;
	int tot;
	int i;
	int N;
	int pid;
	int fd;
	int pidFiglio;
	int status,ret;
	if(argc<4)
	{
		printf("ERRORE: il file %s deve essere invocato con almeno 2 nomi di file e 1 carattere!\n",argv[0]);
		exit(1);
	}

	if(strlen(argv[argc-1])!=1)
	{
		printf("ERRORE: l'ultimo parametro deve essere un singolo carattere!\n");
		exit(2);
	}
	Cx=argv[argc-1][0];
	N=argc-2;
	for(i=0;i<N;i++)
	{
		if((pid=fork())<0)
		{
			printf("ERRORE: errore fork()!\n");
			exit(3);
		}

		else if(pid==0)
		{

			printf("Sono il figlio %d di indice %d...\n",getpid(),i);
			if((fd=open(argv[i+1],O_RDONLY))<0)
			{
				printf("ERRORE: il file %s npn esiste!\n",argv[0]);
				exit(-1);
			}

			tot=contaOccorrenze(fd,Cx);
			
			exit(tot);

		}


	}


	for(i=0;i<N;i++)
	{
		pidFiglio=wait(&status);
		if(pidFiglio<0)
		{
			printf("ERRORE: errore wait()!\n");
			exit(4);
		}
		if((status & 0xFF)!=0)
		{
			printf("Figlio con PID=%d terminato in modo anomalo!\n",pidFiglio);
		}else{
			ret=(int)((status >> 8 ) & 0xFF);
			printf("Il figlio con PID=%d ha ritornato %d\n",pidFiglio,ret);
		}

	}

	exit(0);
}
