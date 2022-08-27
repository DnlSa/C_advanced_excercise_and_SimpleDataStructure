#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>




const char *semName = "asdfsd";
sem_t *sem_id;
void parent(sem_t *sem_id){

    if (sem_id == SEM_FAILED){
        perror("Parent  : [sem_open] Failed\n"); return;
    }
    printf("3)  PADRE  : Aspetto che il figlio stampi \n");

    if (sem_wait(sem_id) < 0) // prelevo il token // mette in attesa il padre
        printf("Parent  : [sem_wait] Failed\n");

    printf("4) PADRE  : il figlio ha stampato \n");

    if (sem_close(sem_id) != 0){ // chiudo il semaforo
        perror("Parent  : [sem_close] Failed\n"); return;
    }
    if (sem_unlink(semName) < 0){ // sgancio il semaforo
        printf("Parent  : [sem_unlink] Failed\n"); return;
    }
}
void child(sem_t *sem_id)
{
   // sem_t *sem_id = sem_open(semName, 0); // creo il semaforo

    if (sem_id == SEM_FAILED){
        perror("Child   : [sem_open] Failed\n"); return;
    }
    printf("1) Figlio    : Io sono pronto ! rilascio il semaforo\n");

    if (sem_post(sem_id) < 0) // Do il token
        printf("Child   : [sem_post] Failed \n");
}

void foo(int sig){

    printf("allarme restituisco il token\n");
    sem_post(sem_id);


}


int main(int argc, char *argv[])
{
    pid_t pid;
    signal(SIGALRM, foo);
    alarm(4);
     sem_id = sem_open(semName, O_CREAT, 0600, 2); // creo il semaforo
     sem_wait(sem_id);
    sem_wait(sem_id);
 sem_wait(sem_id);


    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (!pid){
        child(sem_id);
        printf("2) FIGLIO : ho finito con il semaforo   \n");
    }
    else{
        int status;
        parent(sem_id);
        wait(&status);
        printf("5) PADRE : ho finito con il semaforo \n");
    }
    return 0;
}
