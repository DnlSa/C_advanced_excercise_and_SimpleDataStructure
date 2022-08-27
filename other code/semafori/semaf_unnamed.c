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
#include <sys/mman.h>







sem_t *s1;
sem_t *s2;

void parent(void){

redo:
    sem_wait(s2);
    sleep(1);
    printf("sono il processo PADRE \n");
    sem_post(s1);
goto redo;
}


void child(void)
{
redo:
    sem_wait(s1);
    sleep(1);
    printf("sono il processo FIGLIO \n");
    sem_post(s2);
goto redo;
}


int main(int argc, char *argv[])
{
    pid_t pid;



    s1 = (sem_t *) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    s2 = (sem_t *) mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    sem_init(s1 , 1, 1);
    sem_init(s2 , 1 ,0);

    pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (!pid){
        child();
        printf("2) FIGLIO : ho finito con il semaforo   \n");
    }
    else{

        parent();

        printf("5) PADRE : ho finito con il semaforo \n");
    }
    return 0;
}
