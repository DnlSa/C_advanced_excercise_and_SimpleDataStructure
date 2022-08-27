#include "basic.h"
#define N 5



/*
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


*/

int indice;


int main(void ){

pthread_t tid1  , tid2 , tid3 , tid4;
int i;
int j=0;
/*
la sintassi del sem_init e che
1) passo l indirizzo del semaforo
2) passo il valore dello shared
	0 -> il semaforo e condiviso su thread e processi
	non zero  -> semaforo condiviso attraverso semfori e potrebbe essere bloccato in una regione di memoria condivisa
*/

	pid_t pid[N];
	sem_t  *sem[N]; //N=5
	sem_t  *sem_main; //N=5

	for(i =0 ; i<N ; i++){ // li condivido
		sem[i] = (sem_t *) mmap(NULL , sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS , 0,0);
		}
	sem_main = (sem_t *) mmap(NULL , sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS , 0,0);


	for(i =0 ; i<N; i++) // 0 A 4
		sem_init(sem[i], 1 , 0); // semaforo bloccato

	sem_init(sem_main,  1 , 1); // semaforo bloccato

printf("spawn dei thread\n");

for(i =0 ; i<(N) ; i++){
	pid[i]= fork();

	if(pid[i]==0){

	while (1){
		printf("sono il figlio %d , %d e sono bloccato \n", i , j);
		sem_wait(sem[j]);
		printf("sono il figlio %d e sonon sbloccato \n", i);
		sleep(1);

		j++; // qui vado avanti ogni volta che  che finisco un processo do il token al succiesivo
		if(j==N ){
			sem_post(sem_main);

		}
		sem_post(sem[j]);

	}


	}else if(pid[i]==-1){
		perror("error to create process ");
		exit(-1);



	}else {
			continue;
	}

}
		while(1){
		printf("sono il processo padre aspetto\n");
		sem_wait(sem_main);
		printf("sono il processo padre sbloccato\n");
	//i++; // manda i processi in odrine pero devo tornare su questo processo main
	//	if(i==N )
	//		i=0;
		j=0;
		sem_post(sem[j]);
		}
exit(0);

}



















