#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int fd1,fd2;/* per le due open */
	int piped[2];/* per la creazione della pipe */

	
	if((fd1=open(argv[0],O_RDONLY))<0)
	{
		printf("ERRORE: errore prima open()!\n");
		exit(1);
	}

	printf("fd1 è %d.\n",fd1);

	if((fd2=open(argv[0],O_RDONLY))<0)
	{
		printf("ERRORE: errore seconda open()!\n");
		exit(2);
	}

	printf("fd2 è %d.\n",fd2);

	close(fd1);

	if(pipe(piped)<0)
	{
		printf("ERRORE: errore creazione pipe!\n");
		exit(3);
	}
	printf("Pipe creata con piped[0]=%d e piped[1]=%d.\n",piped[0],piped[1]);

	exit(0);
}
