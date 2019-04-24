#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char **argv){
	
	int n;
	char *c;

	if(argc!=4)
	{
		printf("ERRORE: necessari esattamente 3 parametri!\n");
		exit(1);
	}

	n=atoi(argv[2]);

	if(n<=0){
		printf("ERRORE: il secondo parametro (%d) non è un numero strettamente maggiore di 0!\n",n);
		exit(2);

	}

	c=argv[3];

	if(strlen(c)!=1){
		printf("ERRORE: il terzo parametro non è un singolo carattere!\n");
		exit(3);
	}
	

	printf("Il file %s è invocato con %d parametri:\n",argv[0],argc-1);
	printf("1)\t%s\t(nome di un file)\n",argv[1]);
	printf("2)\t%d\t(umero strettamente positivo)\n",n);
	printf("3)\t%c\t(singolo carattere)\n",*c);



	exit(0);
}

