#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char **argv)
{
	int pid;/* Per la fork */

	if((pid=fork())<0){
		printf("ERRORE: errore fork()!\n");
		exit(1);
	}
	if(pid==0)
	{/* Codice eseguito dal figlio */
		printf("Hello, I'm the child!\t%d\n",pid);
		exit(0);
	}
	/* Codice eseguito dal padre */
	printf("Hello, I'm the father!\t%d\n",pid);
	wait((int *)0);
		
	exit(0);
}
