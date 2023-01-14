/*
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[] i pathname
associati ad N file (F1 ... FN), con N maggiore o uguale ad 1.


Per ognuno di questi file generi un thread che gestira' il contenuto del file.

Dopo aver creato gli N file ed i rispettivi N thread, il main thread dovra'
leggere indefinitamente la sequenza di byte provenienti dallo standard-input.

Ogni 5 nuovi byte letti, questi dovranno essere scritti da uno degli N thread
nel rispettivo file. La consegna dei 5 byte da parte del main thread
dovra' avvenire secondo uno schema round-robin, per cui i primi 5 byte
dovranno essere consegnati al thread in carico di gestire F1, i secondi 5
byte al thread in carico di gestire il F2 e cosi' via secondo uno schema
circolare.



L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra',
a partire dai dati correntemente memorizzati nei file F1 ... FN, ripresentare
sullo standard-output la medesima sequenza di byte di input originariamente
letta dal main thread dallo standard-input.

Qualora non vi sia immissione di input, l'applicazione dovra' utilizzare
non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/

/*
NOTE :

gcc prog.c -o pro -pthread

 ./pro pippo.txt paperino.txt peppa.txt

 ciao mamma come stai io tutto bene spero che tu stia meglio di me




PROBLEMA : legge tutte stringhe in multiplo di 5

 */


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define N_PAGE   1
#define SIZE PAGE_SIZE*N_PAGE
#define N 5
#define MAX_SEM 20


// strittura argomenti (anche questa si puo evitare ma a livello didattico e utile vederla )
struct thread_arg{
	long index ;
	long fd;
};


char pkt[N]; // stringa blobale di 5 byte
int n_arg ; // numero di argomenti passati

struct thread_arg buff[MAX_SEM]; // metto la struttura glrobale perche mi serve per passargli dati alla signal handler
sem_t sem[MAX_SEM]; // i semafori per implementare la round robin (vedere come posso farne un array )
sem_t sem_main;
int sem_index ;

void signal_handler(int signum){
	long i =0 ;
	int count=0;
	char recostruction[SIZE]; //bufferino di appoggio
	int byte_r ;
	for(i=0; i<n_arg; i++)
		lseek(buff[i].fd,0,SEEK_SET); // per tutti i file mi punto a 0 la session data
	i=0;
	while((byte_r = read(buff[i].fd , &recostruction[count] , 5))== 5){ //leggo a blocchi di 5(perche nela thread scrivo a blocchi di 5 )
			count+=byte_r; // contatore dei byte scritti  per scrivere tutto nel recostruction
			i++; // incremento dell indice cosi punto al successivo descrittore
			if(i == n_arg){i=0;} // se arrivo alla fine risetto a 0 l indice
		}
	recostruction[count]= '\0'; // inserisco i terminatore di stringa altrimenti e solamente una sequenza di caratteri
	printf("letti %d\n", count);
	if(byte_r == -1){
		perror("errore ");
		exit(-1);
	}
	printf("le stringhe totali passate sono : %s, %d\n", recostruction, count);
	exit(EXIT_SUCCESS);
}

void main_thread(void){
	sleep(0.5);
	sem_index =0;
	 printf("\n\n");
	 int i;
     int val;
	 char *string;

	while (1){ // FUNZIONA
		printf("sono il thread_main e aspetto\n");
		sem_wait(&sem_main); // prende il semaforo
		printf("sono il thread_main e sono stato sbloccato puoi\ninserire la stringa: ");
		for (i = 0;i<5;i++){pkt[i] = getchar();}


		printf("do il gettone, %s\n", pkt);
		sem_post(&sem[sem_index]); // do il gettone al semaforo (che mi serve )
		sem_index++; // incremento l indice dei semafori
		if(sem_index == n_arg) // se arrivo ad n_arg allora resetto il semaforo
			sem_index = 0;
	}
}

void *open_thread(void *arg){

	char *filename = (char *) arg;
	long fd;
	printf("%s\n", filename);

	if(( fd= open(filename, O_RDWR|O_CREAT|O_TRUNC , 06660))==-1){
		perror("OPEN error");
		exit(-1);
	}
	void *status = (void *)fd;
	pthread_exit(status);
}

void *thread_function(void *arg){
		int byte_w ;
		long j = (long) arg;
		long i=  buff[j].index; // prendo l indice
		long fd = buff[j]. fd; // prendo il descrittore
		printf("sono il processo figlio %ld \n",j);
redo:
		printf("sono il thread %ld aspetto\n", i);
		sem_wait(&sem[i]);
		printf("sono il thread %ld sbloccato, %s\n", i, pkt);
		if((byte_w = write(fd , pkt , 5))<0){
			perror("write error");
			exit(EXIT_FAILURE);
		}
		sem_post(&sem_main);
goto redo;
}

void main(int argc , char **argv){

	if(argc <2){
		printf("usage:  ./prog_name <filename> <filename>.....");
		fflush(stdout);
		exit(0);
	}
	n_arg = argc-1;
	int ret ;
	void *status;
	int i;
	long j=0;

	signal(SIGINT , signal_handler);
	sem_init(&sem_main, 0 ,1 ); // solo il semaforo del thread main sara attivo per leggere la prima stringa e produrre il primo pacchetto
	for(i=0 ; i<n_arg ; i++){
		sem_init(&sem[i], 0 ,0 ); // tutti i semafori in attesa dopo lo spawn dei thread
	}
	pthread_t tid_main;
	pthread_t tid[argc];
	for(i=1 ; i<argc ;i++){
		if((ret=pthread_create(&tid[0], NULL ,open_thread , (void *)argv[i]))){ // il primo thread aprira tutti i file
			perror("errore nella creazione del thread");
			exit(ret);
		}

		pthread_join(tid[0], &status); // la status variabile long
		printf("%ld \n", (long)status);
		buff[j].fd = (long)status;
		buff[j].index = j;
		if((ret=pthread_create(&tid[i], NULL ,thread_function , (void*)j))){ // questi thread lavorano a round robin
			perror("errore nella creazione del thread");
			exit(ret);
		}
	j++;
	}
	main_thread();
}
