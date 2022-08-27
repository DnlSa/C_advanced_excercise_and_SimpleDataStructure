#include "basic.h"

sem_t s1;
sem_t s2; 

void *ping(void *arg){
	while (1){
		sem_wait(&s1);
		sleep(1);
		printf("pong\n");
		sem_post(&s2);
	}

pthread_exit(0);
}

void *pong(void*arg) {	
	while(1){
		sem_wait(&s2);
		sleep(1);
		printf("ping\n");
		sem_post(&s1);
	} 
	pthread_exit(0);
}





int main(void ){

pthread_t tid1  , tid2; 

/*
la sintassi del sem_init e che 
1) passo l indirizzo del semaforo 
2) passo il valore dello shared 
	0 -> il semaforo e condiviso su thread e processi 
	non zero  -> semaforo condiviso attraverso semfori e potrebbe essere bloccato in una regione di memoria condivisa 



*/

sem_init(&s1, 0 , 0); // semaforo bloccato 
sem_init(&s2 ,0, 1); // semaforo sbloccato
	
 pthread_create(&tid1 , NULL , ping , NULL);
 pthread_create(&tid2 , NULL , pong , NULL);


 pthread_join(tid1, NULL);
 pthread_join(tid2 , NULL);
	
}



















