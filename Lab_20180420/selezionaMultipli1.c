#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int fd;/* Per la open */
	int n;/* Per contenere il numero intero */
	char c;/* per contenere il carattere */
	int i;

	long int lunghezza;/* Contiene la lunghezza del file */
	long int pos;/* Per lseek */

	/* Controllo il numero di parametri con cui il file viene invocato */
	if(argc!=3){
		printf("ERRORE: Il file deve essere invocato con esattamente 2 parametri!\n");
		exit(1);
	}

	/* Controllo se il secondo parametro è un numero intero strettamente maggiore di 0 */
	n=atoi(argv[2]);

	if(n<=0){
		printf("ERRORE: Il secondo parametro non è un numero intero strettamente maggiore di 0!\n");
		exit(2);
	}
	
	if((fd=open(argv[1],O_RDONLY))<0){
		printf("ERRORE: Il file %s non esiete!\n",argv[1]);
		exit(3);
	}
	
	printf("Inizio ad eseguire il programma %s...\n",argv[0]);

	lunghezza=lseek(fd,SEEK_SET,SEEK_END);
	
	pos=(long int)0;

	for(i=1;pos<lunghezza;i++)
	{
		pos=(long int)(i*n);
		if(pos<lunghezza)
		{
			lseek(fd,pos-1,SEEK_SET);
			read(fd,&c,1);
			printf("%c\n",c);

		}
		if(pos>lunghezza){
			printf("Il file %s non ha una dimensione multipla di %d\n",argv[1],n);
		}
	}

	exit(0);
}
