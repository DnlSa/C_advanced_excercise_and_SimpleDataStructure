/*
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[1] un numero
intero N maggiore o uguale ad 1 (espresso come una stringa di cifre 
decimali), e generi N nuovi thread.

Ciascuno di questi, a turno, dovra'
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
#define MAX_SEM 4096

typedef struct _data{
	int val;
	struct _data* next;
} data;


// SE DICHAIRO 2 VOLTE LA STESSA COSA FA CASINI non funziona il semaforo perche dichiavaro sia globalmente che dentro lo scope del main


// cose che servono per la lista
data *ptr[MAX_SEM];
int k;
//cose che sevono per il semaforo
sem_t sem[MAX_SEM];
sem_t sem_main;
int indice;

// cose che servono per la signal_handler
int n_threads;

void signal_handler(int signum){
	int i ;
	printf("\n");
	for( i=0; i<n_threads ; i++){
		data *counter = ptr[i]; // devo vedere come passare le teste
		printf("il thread %d\n",i);
		while (counter->next!=NULL){
			printf("%d ->",counter->val);
			counter = counter->next;
		}
			printf("NULL");
			printf("\n");
	}
printf("\n");
exit(EXIT_SUCCESS);
}

void *thread_function(void *arg){

	long i = (long)arg;
	data *head  = NULL;
	head = malloc(sizeof(data)); // do la grandezza al nodo
	head->val = 0; // ci metto il valroe passato
	head->next = NULL; // lo punto a NULL (inserimento in coda )
	ptr[i]= head;

	while (1){ // si faranno inserimenti in coda
		printf("sono il thread %ld aspetto\n", i);
		sem_wait(&sem[indice]);
		printf("sono il thread %ld sbloccato\n", i);
		data *new_node; // il nuovo nodo
		new_node = malloc(sizeof(data)); // do la grandezza al nodo
		new_node->val = k; // ci metto il valroe passato
		new_node->next = NULL; // lo punto a NULL (inserimento in coda )
		if(head==NULL){ // se e il primo inserimento punto la testa al nuovo nodo
			head->next = new_node;
		}else{ // vale se sono inserimenti successivi al primo
			data *counter;
			counter = head; // nodo contatore che scorre la lita
			while (counter->next != NULL){ // scorre finche non arriva all ultimo nodo
				printf("%d ->", counter->val);
				counter = counter->next;
			}
			counter->next= new_node; // puntanto new_node a null puntero il counter che e il nodo precedente al  nuovo nodo
		}
		printf("\ninserimento riuscito\n");
		sem_post(&sem_main); // torno nel main per il prossimo inseriemento
	}
}

int main(int argc , char **argv){
	if(argc != 2 ){
		printf("usage: ./prog n_threads");
		fflush(stdout);
		exit(0);
	}
	n_threads = atoi(argv[1]);

	sem[n_threads];
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
		scanf("%d",&k);// passo l intero
		getc(stdin);
		sem_post(&sem[indice]);
		}
	}
