#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{       char buffer [BUFSIZ];
        int nread, fd = 0;

if (argc > 2) { puts("Errore nel numero di parametri");
                exit(1); }

if (argc == 2)
/* abbiamo un parametro che deve essere considerato il nome di un file */
        if ((fd = open(argv[1], O_RDONLY)) < 0)
               { puts("Errore in apertura file");
                 exit(2); }

/* se non abbiamo un parametro, allora fd rimane uguale a 0 */
while ((nread = read(fd, buffer, BUFSIZ)) > 0 )
/* lettura dal file o dallo standard input fino a che ci sono caratteri */
                write(1, buffer, nread);
/* scrittura sullo standard output dei caratteri letti */
return 0;
}

