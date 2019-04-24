/* 
Si progetti, utilizzando il linguaggio C e le primitive di basso livello che operano sui file, un filtro che accetta tre parametri: il primo parametro deve essere il nome di un file (F), mentre il secondo deve essere considerato un singolo carattere (C) e il terzo un singolo carattere (Cs) da sostituire a C.  Il filtro deve operare una modifica del contenuto del file F: in particolare, tutte le occorrenze del carattere C nel file F devono essere sostituite con il carattere Cs. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{	int fd;
	char c;

if (argc != 4) { puts("Errore nel numero di parametri"); 
                exit(1); }

if ((fd = open(argv[1], O_RDWR)) < 0)
               { puts("Errore in apertura file"); 
                 exit(2); }

if (strlen(argv[2]) != 1)
               { puts("Errore non carattere"); exit(3); }
if (strlen(argv[3])!=1)
{
	puts("Errore non carattere");exit(4);
}
while (read(fd, &c, 1))
          if (c == argv[2][0])
                { lseek(fd, -1L, 1);
         /* SI DEVE RIPORTARE INDIETRO IL FILE POINTER */
                  write(fd,&argv[3][0], 1);
                }
return 0;
}
