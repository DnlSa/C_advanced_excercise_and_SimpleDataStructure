
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>





#define MAX 10
#define SNAME "/sem"
int j;
int n_arg;
/*

void *thread_function(void *arg){

     char *string = (char *) arg;
     sem_t *sem_id = sem_open(string ,O_CREAT , 0666 ,0 );

    printf("sono il thread del semaforo %s\n",string );

    sem_wait(sem_id);

    printf("sono il thread %d", j);
    j++;
    if(j== n_arg){
        j=0;
    }
    pthread_exit(0);
}

*/





int main(void ){

   j=0;
   n_arg = 5;
   pthread_t tid[n_arg];
   int ret ;
   int i = 0;
   void *status;
   char *string;// = "ciao";

    sprintf(string , "/sem%d", i);
    printf("il semaforo %s\n", string);

     sem_t *sem_id = sem_open(string,O_CREAT , 0666 , 0 ); //imposto un array di semafori
//     sem_wait(sem_id);

exit(0);
}
