#include "basic.h"
#define N 5

sem_t sem[N];

void *ping(void *arg){
	while (1){
		sem_wait(&sem[0]);
		sleep(1);
		printf("ping\n");
		sem_post(&sem[1]);
	}
pthread_exit(0);
}

void *pong(void*arg) {	
	while(1){
		sem_wait(&sem[1]);
		sleep(1);
		printf("pong\n");
		sem_post(&sem[2]);
	} 
	pthread_exit(0);
}

void *pang(void*arg) {
	while(1){
		sem_wait(&sem[2]);
		sleep(1);
		printf("pang\n");
		sem_post(&sem[3]);
	}
	pthread_exit(0);
}

void *peng(void*arg) {
	while(1){
		sem_wait(&sem[3]);
		sleep(1);
		printf("peng\n");
		sem_post(&sem[0]);
	}
	pthread_exit(0);
}





int main(void ){

pthread_t tid1  , tid2 , tid3 , tid4;
int i=0;
/*
la sintassi del sem_init e che 
1) passo l indirizzo del semaforo 
2) passo il valore dello shared 
	0 -> il semaforo e condiviso su thread e processi 
	non zero  -> semaforo condiviso attraverso semfori e potrebbe essere bloccato in una regione di memoria condivisa 



*/
	sem_init(&sem[i], 0 , 1); // semaforo bloccato
	for(i =1 ; i<N ; i++)
		sem_init(&sem[i], 0 , 0); // semaforo bloccato


printf("spawn dei thread\n");
 pthread_create(&tid1 , NULL , ping , NULL);
 pthread_create(&tid2 , NULL , pong , NULL);
 pthread_create(&tid3 , NULL , pang , NULL);
 pthread_create(&tid4 , NULL , peng , NULL);

 pthread_join(tid1, NULL);
 pthread_join(tid2 , NULL);
 pthread_join(tid3, NULL);
 pthread_join(tid4 , NULL);
	
}



















