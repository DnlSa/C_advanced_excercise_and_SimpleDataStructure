/*
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[1] un numero
intero N maggiore o uguale ad 1 (espresso come una stringa di cifre
decimali), e generi N nuovi thread. Ciascuno di questi, a turno, dovra'
inserire in una propria lista basata su memoria dinamica un record
strutturato come segue:

typedef struct _data{
	int val;
	struct _data* next;
} data;

I record vengono generati e popolati dal main thread, il quale rimane
in attesa indefinita di valori interi da standard input. Ad ogni nuovo
valore letto avverra' la generazione di un nuovo record, che verra'
inserito da uno degli N thread nella sua lista.


L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra'
stampare a terminale il contenuto corrente di tutte le liste (ovvero
i valori interi presenti nei record correntemente registrati nelle liste
di tutti gli N thread).

*****************************************************************/

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_SIZE 4096
#define MAX_SEM 20

typedef struct _data{
	int val;
	struct _data* next;
} data;


sem_t sem[MAX_SEM];
sem_t sem_main;
int k;
int indice;


char buff[MAX_SIZE];

void signal_handler(int signum){

printf("termino processo \n");
	raise(SIGTERM);
}



void *thread_function(void *arg){

	long i = (long)arg;
	while (1){

		printf("sono il thread %ld aspetto\n", i);
		sem_wait(&sem[indice]);
		printf("sono il thread %ld sbloccato\n", i);

	// OPERAZIONI VARIE
		sleep(0.5);
		printf("esco\n");
		sem_post(&sem_main);
	}
}



int main(int argc , char **argv){
	if(argc != 2 ){
		printf("usage: ./prog n_threads");
		fflush(stdout);
		exit(0);
	}

	int n_threads = atoi(argv[1]);
	sem_t sem[n_threads];

	long i;
	int ret;

	signal(SIGINT , signal_handler);
	// inizializzo i semafori
	sem_init(&sem_main, 0,1); // faccio partire prima il thread main che deve leggere gli interi
	for(i=0 ; i<n_threads ; i++){
		sem_init(&sem[i], 0 , 0);
	}

	pthread_t tid[n_threads];
	for(i=0; i<n_threads ; i++){
		if((ret = pthread_create(&tid[i], NULL , thread_function , (void *)i))!=0){ // vedere il NULL se tocca passargli qualcos altro
			perror("error to create thread");
			exit(-1);

		}
	}
indice =0 ;

	while (1){
		sleep(1);
		sem_wait(&sem_main);
		printf("inserisci una valore : ");
		scanf("%[^\n]", buff);// passo l intero
		getc(stdin);

		indice ++ ;
		printf("esco %d\n", indice);
		sem_post(&sem[indice]);
	}

}
