#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	
	if(argc<=1){
		printf("Errore: inserire almeno un parametro!\n");
		exit(1);
	}


	int i;
	for(i=1;i<argc;i++){
		printf("Il parametro #%d è %s\n",i,argv[i]);
	}

	exit(0);
}
