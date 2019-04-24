#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int n;/* Contiene il numero di linee */
	int i;/* contatore */
	char c;
	int fd;
	/* Controllo il numero di parametri */
	if(argc>3){
		printf("ERRORE: Il programma deve essere invocato con 0,1 o 2 parametri!\n");
		exit(1);
	}
	
	fd=0;
	switch(argc){
		case 2:	
			if(argv[1][0]!='-')
			{
				if((fd=open(argv[1],O_RDONLY))<0)
				{
					printf("ERRORE: Il file non esiste!\n");
					exit(2);
				}
				n=10;
			}else{
				n=atoi(&(argv[1][1]));
			}
			break;
		case 3:
			if((fd=open(argv[2],O_RDONLY))<0)
			{
			       printf("ERRORE: Il file non esiste!\n");
			       exit(2);
			}
			n=atoi(&(argv[1][1]));
			break;

		default: 
			 n=10;
			 break;



	}
	i=0;	
	while(i<n && read(fd,&c,1)){
		if(c=='\n') i++;
		write(1,&c,1);
	}
	exit(0);
}
