
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

char *filename = "prova.txt";
FILE *file;
int numeroThreads = 5;
char **memory_segments;



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

	pthread_t tid;
	pid_t pid;

	

	memory_segments = malloc(sizeof((char*)*numeroThreads)); // numero di segmenti
	if (memory_segments == NULL){
		printf("memory alloction error (1)\n");
		return -1;
	}

	// creo tante aree di memroia quanto sono i thread
	for (int i = 0; i < numeroThreads; i++) { // mappa la memoria . avendo tutti questi indirizzi poi li passo al ai thread
		memory_segments[i] = (char *)mmap(NULL, SHMEMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0) ; 
		if(memory_segments[i] == NULL){
			printf("mmap error\n");
			return -1;
		}
	}

	file = fopen(filename, "w+");
	sem_t *sem_id = sem_open(SNAME , O_CREAT , 0666, 0);
	pid = fork();


	if(pid == 0){

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

		for (long i = 0; i < numeroThreads; i++) {
			if (pthread_create(&tid, NULL, functA, (void *)i) == -1) {
				printf("pthread_create error (parent)\n");
				return -1;
			}
			pthread_join(tid, NULL);
		}
		sem_post(sem_id);
	}

	exit(0);

}
