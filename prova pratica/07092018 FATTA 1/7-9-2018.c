/******************************************************************

SPECIFICATION TO BE IMPLEMENTED: 
Implementare un'applicazione che riceva in input tramite argv[] il 
nome di un file F ed una stringa indicante un valore numerico N maggiore
o uguale ad 1.
L'applicazione, una volta lanciata dovra' creare il file F ed attivare 
N thread. Inoltre, l'applicazione dovra' anche attivare un processo 
figlio, in cui vengano attivati altri N thread. 
I due processi che risulteranno attivi verranno per comodita' identificati
come A (il padre) e B (il figlio) nella successiva descrizione.

Ciascun thread del processo A leggera' stringhe da standard input. 
Ogni stringa letta dovra' essere comunicata al corrispettivo thread 
del processo B tramite memoria condivisa, e questo la scrivera' su una 
nuova linea del file F. Per semplicita' si assuma che ogni stringa non
ecceda la taglia di 4KB. 

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo A venga colpito esso dovra' 
inviare la stessa segnalazione verso il processo B. Se invece ad essere 
colpito e' il processo B, questo dovra' riversare su standard output il 
contenuto corrente del file F.

Qalora non vi sia immissione di input, l'applicazione dovra' utilizzare 
non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHMEMSIZE 4096




# define SNAME "/semaphore"
char *fileName;
FILE *file;
int numeroThreads;
int sblocco = 0;
char **memory_segments;
pid_t pid;

void parent_handler(int signo)
{
	printf("Il padre ha ricevuto il segnale lo inoltra al figlio\n");
	kill(pid, signo);
}

void child_handler(int dummy)
{
	char *buffer;
	while(fscanf(file, "%ms", &buffer) != EOF)
	{
		printf("%s\n", buffer);
		free(buffer);
	}
	
}

void *functB(void *indice)
{
	long index = (long)indice;

		printf("Il thread lettore numero %ld sta accedendo alla memoria...\n", index);
		fflush(stdout);
		fprintf(file, "%s\n", memory_segments[index]);
		fflush(file);
		printf("ho scritto nel file \n");
	pthread_exit(0);
}

void *functA(void *i)
{
	long index = (long)i;
		printf("Il thread scrittore numero %ld sta accedendo alla memoria... : ", index);
		scanf("%[^\n]", memory_segments[index]); //posiziono la stringa sulla memoria
		getc(stdin);
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	fileName = argv[1];
	numeroThreads = atoi(argv[2]);
	pthread_t tid;
	
	memory_segments = malloc(sizeof(char*)*numeroThreads);

	if (memory_segments == NULL){
		printf("memory alloction error (1)\n");
		return -1;
	}

	for (int i = 0; i < numeroThreads; i++) { // crea n memorie condivise  il cui indirizzo
		memory_segments[i] = (char *)mmap(NULL, SHMEMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0) ; 
		if(memory_segments[i] == NULL){
			printf("mmap error\n");
			return -1;
		}
	}

	file = fopen(fileName, "w+");

sem_t *sem_id = sem_open(SNAME , O_CREAT , 0666, 0);

	pid = fork();


	if(pid == 0){
		signal(SIGINT,child_handler);
		sem_wait(sem_id);
		for (long i = 0; i < numeroThreads; i++) {
			if (pthread_create(&tid, NULL, functB, (void *)i ) == -1) {
				printf("pthread_create error (child)\n");
				return -1;
			}
			pthread_join(tid, NULL);
		}
		exit(0);
	}
	else if (pid  == -1) {
		printf("fork error\n");
		return -1;
	}
	else{
		signal(SIGINT,parent_handler);
		for (long i = 0; i < numeroThreads; i++) {
			if (pthread_create(&tid, NULL, functA, (void *)i) == -1) {
				printf("pthread_create error (parent)\n");
				return -1;
			}
			pthread_join(tid, NULL);
		}
		sem_post(sem_id);
	}

	while(1) pause();

}
