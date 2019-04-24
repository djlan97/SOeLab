#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv){
	char Cx; /* Il carattere da cercare nel file */
	char c; /* Variabile per leggere ogni singolo crattere del file */
	int fd; /* Per la open */
	long int tot=0; /* contatore per le occorrenze */

	/* CONTROLLO DEI PARAMETRI */

	if(argc!=3){
		printf("ERRORE: il file %s deve essere invocato con esattamente 2 parametri!\n",argv[0]);
		exit(1);
	}
	
	if(strlen(argv[2])!=1){
		printf("ERRORE: il secondo parametro deve essere un carattere!\n");
		exit(2);
	}

	Cx=argv[2][0];
	
	/* ESERCIZIO */
	
	/* Apriamo il file */
	
	if((fd = open(argv[1],O_RDONLY))<0){
		printf("ERRORE: il file non esiste!\n");
		exit(3);
	}
	
	/* Leggo il file */
	while(read(fd,&c,sizeof(c))!=0){
		if(c==Cx){ 
			tot++;
		}
	}

	printf("Il numero di occorrenze di %c nel file %s è %ld\n",Cx,argv[1],tot);

	exit(0);
}
