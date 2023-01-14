/*
 Si sviluppi una applicazione che riceva tramite argv[] la seguente linea di comando

    nome_prog -f file1 [file2] ... [fileN] -s stringa1 [stringa2] ... [stringaN] 
    
indicante N nomi di file (con N > 0) ed N ulteriori stringhe (il numero dei nomi dei 
file specificati deve corrispondere al numero delle stringhe specificate).




L'applicazione dovra' generare N processi figli concorrenti, in cui l'i-esimo di questi 
processi effettuera' la gestione dell'i-esimo dei file identificati tramite argv[].
Tale file dovra' essere rigenerato allo startup dell'applicazione.

Il main thread del processo originale dovra' leggere indefinitamente stringhe da 
standard input e dovra' comparare ogni stringa letta con le N stringhe ricevute in 
input tramite argv[].



Nel caso in cui la stringa letta sia uguale alla i-esima delle N stringhe ricevuta 
in input, questa dovra' essere comunicata all'i-esimo processo figlio in modo che questo 
la possa inserire in una linea del file di cui sta effettuando la gestione.

Invece,
se il main thread legge una stringa non uguale ad alcuna delle N stringhe ricevute 
in input, questa stringa dovra essere comunicata a tutti gli N processi figli
attivi, che la dovranno scrivere sui relativi file in una nuova linea.






In caso non vi sia immissione di dati sullo standard input e non vi siano segnalazioni, 
l'applicazione dovra' utilizzare non piu' del 5% della capacita' di lavoro della CPU.*/



/*
 gcc prog.c -o prog
 ./prog -f primo.txt secondo.txt terzo.txt quarto.txt quinto.txt  -s casa erba meridiana cancello apostrofo

 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <signal.h>


#define MAX_SIZE 4096
#define MAX_ARG 100

struct info {
	pid_t pid;
	int fd;
	char *filename;
};

struct info n_proc[MAX_SIZE];
int n_arg;


void signal_handler(int sigint){
	int i;
	int fd;
	int fd_bck;
	int count;
	int byte_r , byte_w;
	char string[MAX_SIZE];
	char bufferino[MAX_SIZE];
	pid_t pid_ret = getpid();

	for(i=0 ; i<n_arg ; i++){
		if(pid_ret == n_proc[i].pid){
			close( n_proc[i].fd) ;
			break;
		}
		else
			;
	}
sprintf(string , "%s_backup.txt", n_proc[i].filename); // apro il file con _backup
	if((fd_bck = open(string, O_CREAT|O_TRUNC|O_RDWR , 06660))==-1){
				perror("error to OPEN file");
				exit(-1);
			}
	if((fd = open(n_proc[i].filename, O_CREAT|O_RDONLY , 06660))==-1){
				perror("error to OPEN file");
				exit(-1);
			}

	i=0;
	while ((byte_r = read(fd , &bufferino[i] ,1))==1){
			i++;
	}
	byte_w= write(fd_bck , &bufferino , strlen(string));
	printf("sono stati scritti %d\n", i );

close(fd);
close(fd_bck);
raise(SIGTERM);
}




int main(int argc , char **argv){
	if(argc<2){
		printf("usage: nome_prog -f file1 [file2] ... [fileN] -s stringa1 [stringa2] ... [stringaN] " );
		fflush(stdout);
		exit(0);
	}
	int i;
	n_arg = (argc-3)/2;
	printf("%d\n", n_arg);
	int j;
	char *filename[n_arg];
	char *string[n_arg];
	int k;

	char *mem_shared =(char *)mmap(NULL, MAX_SIZE ,PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS , 0,0 ); // instanzio una memoria condivisa fra tutti i procssi (questi erediteranno l indirizzo )

	for(k=0 , i=2; k<n_arg ;k++,i++){
		filename[k]=argv[i];
		string[k] = argv[i+(n_arg+1)];
	}
		for(i=0 ; i<n_arg;i++){
			printf("%s , %s \n", filename[i], string[i]);
		}

	// impostaiamo i semafori

	sem_t *sem[n_arg+1];
	for(i=0 ; i<n_arg;i++)
		sem[i]=(sem_t *)mmap(NULL , sizeof(sem_t), PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS , 0,0 );
	sem[n_arg]=(sem_t *)mmap(NULL , sizeof(sem_t), PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS , 0,0 );

	for(i=0 ; i<n_arg;i++)
		sem_init(sem[i], 1, 0);
	sem_init(sem[n_arg], 1,1);


	//sapwn dei nuovo procesi
	pid_t pid[n_arg];
	for(i=0; i<n_arg ; i++){
		pid[i]= fork();
		if(!pid[i]){ //processo figli
			signal(SIGINT ,signal_handler);
			int fd; // apertura del file ;
			int byte_w;
			n_proc[i].pid = getpid();
			n_proc[i].fd = fd;
			n_proc[i].filename = filename[i];
			if((fd = open(filename[i], O_CREAT|O_TRUNC|O_RDWR , 06660))==-1){
				perror("error to OPEN file");
				exit(-1);
			}
		redo:
			printf("sono il figlio %d aspetto\n", i);
			sem_wait(sem[i]);
			printf("sono il figlio %d sbloccato\n", i);
			if((byte_w = write(fd , mem_shared, strlen(mem_shared)))==-1){
				perror("error to write file");
				exit(-1);
			}
			dprintf(fd , "\n"); // aggiungo nuova linea
			printf("sono il figlio %d ho scritto %d byte\n", i , byte_w);


		goto redo;

		}else if(pid[i]== -1){
			perror("errore nella fork");
			exit(-1);
		}

		else{
			continue;
		}
	}
		int ret;
		char buff[MAX_SIZE];

read_again:
			sleep(1);
			printf("sono il processo main\n");
			sem_wait(sem[n_arg]);

			printf("inserisci un astringa :  " );
			scanf("%[^\n]", buff);
			getc(stdin);// brucia l ultimo carattere il new_line
			for(i=0; i<n_arg ; i++){
				if((ret=strcmp(buff , string[i] ))==0){
					memcpy(mem_shared , &buff , strlen(buff));
					sem_post(sem[i]);
					sem_post(sem[n_arg]);
					goto read_again;
				}
			}for(i=0; i<n_arg ; i++){
				memcpy(mem_shared , &buff , strlen(buff));
				sem_post(sem[i]);
				sem_post(sem[n_arg]);
		}
		goto read_again;
}

