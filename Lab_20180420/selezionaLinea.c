#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int n;
	int fd;
	char c;
	int i;
	int find;
	/* controllo il numero di parametri */
	if(argc!=3){
		printf("ERRORE:necessari esattamente 2 parametri!\n");
		exit(1);
	}
	n=atoi(argv[2]);
	if(n<=0){
		printf("ERRORE: il secondo parametro deve essere un numero strettamente amggiore di 0!\n");
		exit(2);
	}

	if((fd=open(argv[1],O_RDONLY))<0){
		printf("ERRORE: il file %s non esiste!\n",argv[1]);
		exit(3);
	}
	i=1;
	find=0;
	while(read(fd,&c,1)!=0){
		if(c=='\n') i++;
		if(i==n){
			find=1;
			printf("La riga %d del file %s è:\n",n,argv[1]);
			while(read(fd,&c,1)!=0){
				write(1,&c,1);
				if(c=='\n') break;
			
			}
			break;
		}
	
	}
	
	if(!find){
		printf("Nel file %s non è presente la riga %d!\n",argv[1],n);
	}

	exit(0);
}
