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

char *fileName = "prova.txt";
FILE *file;
int numeroThreads = 5;
int sblocco = 0;


char **memory_segments;
pid_t pid;


void *functB(void *indice)
{
	long i = (long)indice;

	while(1)
	{
		pthread_mutex_lock((&mutexB)+i);

		printf("Il thread lettore numero %ld sta accedendo alla memoria...\n", i);
		fflush(stdout);

		fprintf(file, "%s\n", memory_segments[i]);
		fflush(file);

		pthread_mutex_unlock((&mutexA)+i);
	}
}

void *functA(void *index)
{
	long i = (long)index;

	while(1)
	{
		pthread_mutex_lock((&mutexA)+i);

		printf("Il thread scrittore numero %ld sta accedendo alla memoria...\n", i);
		scanf("%s", memory_segments[i]); //posiziono la stringa sulla memoria

		pthread_mutex_unlock((&mutexB)+i);
	}
}

int main(void)
{

	pthread_t tid;


/////////////////////////////// crea memoria condivisa
	memory_segments = malloc(sizeof(char*)*numeroThreads);
	if (memory_segments == NULL){
		printf("memory alloction error (1)\n");
		return -1;
	}
	for (int i = 0; i < numeroThreads; i++) {
		memory_segments[i] = (char *)mmap(NULL, SHMEMSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0) ;
		if(memory_segments[i] == NULL){
			printf("mmap error\n");
			return -1;
		}
	}
//////////////////////////////////////////////////
	file = fopen(fileName, "w+");


    pthread_mutex_t mutexA;
    pthread_mutex_t mutexB;

	mutexA = (pthread_mutex_t *)malloc (sizeof(pthread_mutex_t)); //???????????
	mutexB =(pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));


	pthread_mutex_init(&mutexA, NULL); //inizializzo il mutex A
	pthread_mutex_init(&mutexB, NULL); //inizializzo il mutex B

	pthread_mutex_lock(&mutexB); //pongo a zero il mutex B



	if ((pid = fork()) == -1) {
		printf("fork error\n");
		return -1;
	}

	if(pid == 0){

		for (long i = 0; i < numeroThreads; i++) {
			if (pthread_create(&tid, NULL, functB, (void *)i ) == -1) {
				printf("pthread_create error (child)\n");
				return -1;
			}
		}

	}
	else{

		for (long i = 0; i < numeroThreads; i++) {
			if (pthread_create(&tid, NULL, functA, (void *)i) == -1) {
				printf("pthread_create error (parent)\n");
				return -1;
			}
		}

	}

	while(1) pause();

}
