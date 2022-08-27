#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


pthread_mutex_t m1;
pthread_mutex_t m2;

void *ping(void *arg){
	while (1){
		pthread_mutex_lock(&m1);
		sleep(1);
		printf("pong\n");
		pthread_mutex_unlock(&m2);
	}
pthread_exit(0);
}

void *pong(void*arg) {	
	while(1){
		pthread_mutex_lock(&m2);
		sleep(1);
		printf("ping\n");
		pthread_mutex_unlock(&m1);
	} 
	pthread_exit(0);
}





int main(void )
{

pthread_t tid1 ;
pthread_t tid2; 

pthread_mutex_init(&m1, NULL);
pthread_mutex_init(&m2, NULL); // macro 

pthread_mutex_lock(&m2);


 pthread_create(&tid1 , NULL , ping , NULL);
 pthread_create(&tid2 , NULL , pong , NULL);


 pthread_join(tid1, NULL);
 pthread_join(tid2 , NULL);
	
}
