/* Soluzione parte C:
per la sincronizzazione dei figli occorre creare uno schema di pipe, ossia una pipe che colleghi ogni figlio a quello precedente e successivo. Si noti che il primo processo figlio ha la pipe collegata al padre, e che l'ultimo processo figlio parte in maniera indipendente. 
Si scrivera' sempre sulla pipe i e si leggera' da quello (i+1):
figlio N-1  == pipe N-1 ==>   figlio N-2  == pipe N-2 ==> ... 
                                   figlio i == pipe i ==>
in particolare il primo figlio e' collegato al padre tramite la pipe 0 */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

/* definisco un tipo per un array di due interi */
typedef int pipe_t[2];		 

/* definizione di una struttura dati che racchiuda il numero corrispondente al carattere numerico e il PID del processo figlio corrente. Questa struttura verra' passata tramite le pipe da un figlio all'altro. */
typedef struct {
	int PID; 	/* ha il significato del campo c1 del testo */
	int numero;	/* ha il significato del campo c2 del testo */
		} s_num;

/* Inizio del programma vero e proprio */
int main(int argc, char **argv) {
int N;		/* Numero di processi figli da creare */
int i;		/* Contatore per la generazione delle pipe/figli*/
int j;		/* Contatore per la chiusura dei lati non usati delle pipe */
pipe_t *p;	/* Array di pipe usate a pipeline da ultimo figlio, al penultimo figlio .... primo figlio e poi a padre: ogni processo (a parte il primo) legge dalla pipe i+1 e scrive sulla pipe i */
char ch;   	/* carattere utilizzato nella read del file*/
int status; 	/* utilizzato come status di ritorno dei figli*/ 
int fd; 	/* file descriptor generico */
int pid;     	/* valore di ritorno della fork o pid del processo corrente */ 
s_num cur_num;	/* struttura che ogni figlio deve usare */
s_num pipe_num;	/* struttura che verra' letta dalla pipe */
char ritorno;	/* variabile per valore di ritorno per maggiore chiarezza, anche se in questo caso non serviva */

/* controllo sul numero dei parametri: non richiesto ma non ha senso se non abbiamo almeno due file e quindi almeno tre argomenti (argv[0] compreso) */
if (argc < 3) 
{ 	printf("Uso: %s num variabile di file, almeno 2\n", argv[0]);
	exit(1);
}

/* il numero di figli da generare e' pari al numero di file passati come argomento, quindi argc-1 */
N = argc-1;

/* allocazione delle pipe: occorre un numero di pipe pari al numero di figli (una viene usata per la comunicazione con il padre) */    
p = (pipe_t *)malloc(sizeof(pipe_t)*N);  
if ( p == NULL ) 
{
	printf("Impossibile allocare il vettore di pipe\n");
	exit(2);
}

for (i=0; i<N; i++) 
{
	if (pipe(p[i]) != 0) 
	{
		printf("Errore nella creazione della pipe %d\n", i);
		exit(3);
	}
}

/* creazione degli N figli. Dove la fork ritorna 0 significa che sono nel codice del figlio, altrimenti nel padre o in errore se ho -1 */
for (i=0; i<N; i++)  /* ATTENZIONE LA CREAZIONE VIENE FATTA NEL SOLITO MODO DATO CHE LA SOLUZIONE DEVE CONSIDERARE CHE LA PIPELINE PARTE DALL'ULTIMO PROCESSO CREATO, IN TERMINI TEMPORALI */
{
	pid = fork();
	/* controllo del valore di ritorno della fork */
	if (pid == -1)
	{
		printf("Errore nella fork %d-esima\n",i);
		exit(4);
	}
	else
	if (pid == 0)
	{
	/* CODICE DEL FIGLIO */
		printf("Sono il figlio %d e sono associato al file %s\n", getpid(), argv[i+1]);
	/* anzitutto chiudo i lati della pipe che non uso con questo figlio. Chiudo in scrittura tutte le pipe eccetto la i, chiudo in lettura tutte le pipe eccetto i+1 */
	/* chiudo tutte le pipe non usate */  
		for (j=0; j<N; j++) 
		{
			if ( j!=i && j!=(i+1))
			{
				close(p[j][0]);		/* lato lettura */
				close(p[j][1]);		/* lato scrittura */
			}
		}
	/* non devo leggere dalla pipe i-esima */
		close(p[i][0]);	
        /* non devo scrivere sulla pipe i+1: usiamo l'operatore % con valore N in modo che per il figlio N-1esimo si vada a chiudere il lato scrittura della pipe p[0]  */
                close(p[(i+1)%N][1]);

	/* apro il file i-esimo e leggo un carattere numerico alla volta e lo converto */
		fd = open(argv[i+1], O_RDONLY);   
		if (fd < 0) 
		{	
			printf("Errore nell'apertura del file %s\n", argv[i+1]);
			exit(-1); /* decidiamo di tornare -1 che verra' interpretatto dal padre come 255 che non e' un valore accettabile per il padre */
		}
		while (read(fd,&ch,1)) 
		{ 
		/* inizializzo la mia struttura dati: ATTENZIONE farlo dentro al while dato che puo' essere modificata */
			cur_num.PID = getpid();
			cur_num.numero=(int) (ch - '0');
 			/*
			printf("ho letto numero=%d e pid=%d\n", cur_num.numero, cur_num.PID);
			*/
			/* ora posso comunicare al figlio precedente la mia informazione. ATTENZIONE: il figlio N-1esimo non deve leggere da nessuna pipe, ma gli altri figli devono prima confrontare la loro informazione con quella ricevuta sulla pipe */
			if (i != N-1)
			{
				/* se non sono l'ultimo processo, resto bloccato sulla read fino a quando non mi viene passato un qualche dato */
				read(p[i+1][0],&pipe_num,sizeof(s_num));
				/* 
				printf("ho ricevuto numero=%d e pid=%d\n", pipe_num.numero, pipe_num.PID);
 				*/
				/* confronto i dati ricevuti */
				if (pipe_num.numero >= cur_num.numero)
				{
					cur_num.numero = pipe_num.numero;
					cur_num.PID = pipe_num.PID;
				}	
			}	
			/* invio i dati */
			/* 
			printf("sto per inviare numero=%d e pid=%d\n", cur_num.numero, cur_num.PID);
			*/
			write(p[i][1],&cur_num,sizeof(s_num));
		}	

		/* salvo il valore di ritorno anche se non serve, per maggiore leggibilita' */
		ritorno =  ch;
		/* termino il processo ritornando l'ultimo carattere (che e' sicuramente minore di 255) */
		exit(ritorno);
	}	/* FINE CODICE DEL FIGLIO */
}	/* fine del ciclo for */ 

/* CODICE DEL PADRE */
/* chiudo tutte le pipe non usate */  
for (j=0;j<N;j++)  
{
	if( j != 0 )
		/* non devo chiudere la pipe lettura dell'ultimo figlio */          
		close (p[j][0]);
	close (p[j][1]);
} 

/* leggo i valori passati dal primo figlio fino a che ce ne sono */
while (read(p[0][0],&cur_num,sizeof(s_num)))
{
	printf("Numero calcolato: %d dal figlio pid=%d\n",cur_num.numero, cur_num.PID);    
} 

/* Attesa della terminazione dei figli */
for (i=0;i<N;i++) 
{
	pid = wait(&status);
	if (pid < 0) 
		printf("Errore in wait\n");
	else
	{
		if(WIFEXITED(status)) /* se il figlio e` uscito normalmente... */
			printf("Figlio pid: %d terminato con exit: %c\n", pid, WEXITSTATUS(status));
		else
			printf("Figlio con pid: %d terminato in modo anomalo\n", pid);
	}
		
}
exit(0);
}	/* fine del main */
