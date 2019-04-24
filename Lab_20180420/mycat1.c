#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{       char buffer [BUFSIZ];
        int nread, fd = 0;

int i;
for(i=1;i<argc;i++){
	/* ogni parametro deve essere considerato il nome di un file */
	        if ((fd = open(argv[i], O_RDONLY)) < 0)
	        { printf("Errore in apertura file %s",argv[i]);
        	         exit(2); 
	        }
	if(argc>2){
		printf("::::::::::::::\n%s\n::::::::::::::\n",argv[1]);
	}
	/* se non abbiamo un parametro, allora fd rimane uguale a 0 */
	while ((nread = read(fd, buffer, BUFSIZ)) > 0 )
	/* lettura dal file o dallo standard input fino a che ci sono caratteri */
        	        write(1, buffer, nread);
	/* scrittura sullo standard output dei caratteri letti */
}

return 0;
}

