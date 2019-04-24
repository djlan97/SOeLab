#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int n;
	int fd;
	char c;
	int conta;
	char *buf=NULL;
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
	
	
	conta=0;

	while(read(fd,&c,1)!=0){
		conta++;
		buf=realloc(buf,conta);
		buf[conta-1]=c;	
		if(c=='\n'){
			if(conta==n){
				write(1,buf,n);
			}
			conta=0;
		}
	
	}
	free(buf);
	exit(0);
}
