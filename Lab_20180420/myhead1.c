#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int n;/* Contiene il numero di linee */
	int i;/* contatore */
	char c;
	/* Controllo il numero di parametri */
	if(argc!=2){
		printf("ERRORE: Il programma deve essere invocato con esattemente 1 parametro!\n");
		exit(1);
	}

	if(argv[1][0]!='-'){
		printf("ERRORE: Programma invocato in modo errato!\nUsage: %s -n\n",argv[0]);
		exit(2);
	}

	n=atoi(&(argv[1][1]));
	i=0;	
	while(i<n && read(0,&c,1)){
		if(c=='\n') i++;
		write(1,&c,1);
	}
	exit(0);
}
