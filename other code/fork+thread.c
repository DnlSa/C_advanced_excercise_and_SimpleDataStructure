#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
 ERRORE COMPIUTO IDIOTA ERA SETTARE MALE LA OPEN ;

 */



void *thread_function(void * arg){
    char *filename;
    filename = (char *) arg ;
    int res = open(filename,O_CREAT|O_RDWR|O_TRUNC , 06660);
    if(res == -1){
        perror("errore nella creazione del thread");
        res =0 ;
    }
    pthread_exit((void *)res);
}

int main(void){

    pthread_t tid;
    void *stats ;
    char *filename = "ciao";
    pid_t pid;
    int ds;
    int status;

    if((pthread_create(&tid , NULL , thread_function, (void *) filename))==-1){
            perror("failure to create thread");
            exit(-1);
    }
    pthread_join(tid , &stats);
    printf("il descrittore del file e :  %d\n " ,ds = (int)stats);

    if((pid = fork())==-1){
        printf("errore nello spawn del processo figlio\n ");
        exit(-1);
    }
    else if(pid == 0 ){
        printf("sono il processo figlio:  ");
        char *stringa;
        scanf("%m[^\n]",&stringa);
        int n = strlen(stringa);
        write(ds , stringa, n );
        free(stringa);
        close(ds);
        exit(n);

    }else{
        wait(&status);
        printf("sono il processo padre\n");
        lseek(ds , 0 , SEEK_SET);// spiazzati dall inizio del file di 0 byte
        int aux = WEXITSTATUS(status); // frega la terminazione del processo figlio
        int len = (size_t ) aux;
        char buffer[len];
        read(ds, &buffer , len);
        buffer[len]='\0';
        close(ds);
        printf("%s , %d\n" , buffer, len);
        exit(0);
    }
}
