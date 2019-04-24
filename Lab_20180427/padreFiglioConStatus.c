#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char **argv)
{
	int pid;/* Per la fork */
	int status;/* Per la wait */
	int pidFiglio;/* contiene il pid del figlio */
	int exit_s;
	if((pid=fork())<0){
		printf("ERRORE: errore fork()!\n");
		exit(1);
	}
	if(pid==0)
	{/* Codice esegito dal figlio */
		printf("Hello, I'm the child!\t%d\n",pid);
		exit(0);
	}
	/* Codice eseguito dal padre */
	printf("Hello, I'm the father!\t%d\n",pid);
		
	if((pidFiglio=wait(&status))<0){
		printf("ERRORE: errore wait()!\n");
		exit(2);
	}
	if(pid == pidFiglio)
	{
		printf("Terminato il figlio con PID=%d.\n",pidFiglio);
	}else exit(3);

	if((status & 0xFF)!=0)
	{
		printf("Il figlio è terminato involontariamente!\n");
	}else{
		exit_s=status >> 8;/* Prendo gli 8 bit più significativi */
		exit_s &= 0xFF;
		printf("Per il figlio %d lo stato exit è %d\n", pidFiglio,exit_s);
	}

	exit(0);

}
