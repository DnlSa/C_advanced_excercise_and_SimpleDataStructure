
/*
 si e realizzato un codice che adotta

 1) open
 2) thread
 3) processi
 4) pipe


 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

void *string_tokenize( void *arg2){

    // cast dei argomenti
    long fd_pipe = (long) arg2;
    int ret;
    int k;
    char buff;

    int fd = open("filename.txt", O_CREAT |O_TRUNC |O_RDWR , 06660); // apre il file
        if(fd == -1){
            perror("error to open file ");
            exit(EXIT_FAILURE);
        }
    while (ret = read(fd_pipe , &buff , 1) >0) // gira finche non ha copiato tutto
                 write (fd , &buff , 1 );

    write(fd , "\0", 1); // aggiungo terminatore di stringa
    if(ret == -1){//caso di fallimento
        perror("errore to write buffer");
        exit(-1);
    }

    close(fd_pipe);
    close(fd);
    pthread_exit(NULL);
}

int main (void ){

    int pipefd[2]; // quando invoco la pipe mi ritorna 2 descrittori in lettura e scrittura
    pid_t cpid;
    char buf;
    size_t count  = 32;

    if(pipe(pipefd) == -1){
        perror("error to create pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if(cpid == -1){
        perror("eroor to create process ");
        exit(EXIT_FAILURE);
    }


    if(cpid == 0){ // il processo figlio ha la stringa ospitata nella pipe
        pthread_t tid; // faccio spawnare un thread che mi tokenizza al strigna
        close(pipefd[1]) ; // chiudo il canale in scrittura

        if(pthread_create(&tid , NULL , string_tokenize ,(void *)pipefd[0])){ // do al thread il canale di lettura della pipe il puntatore al buf
            printf("errore nella creazione del thread\n");
            fflush(stdout);
            exit(EXIT_FAILURE);
        }

        pthread_join(tid , NULL); // la thread function mi deve tornare l'indirizzo al primo elemento del vettore di puntatori.


        close (pipefd[0]); // chiudo il canale di lettura della pipe
        exit(EXIT_SUCCESS); // uscida dal processo figlio


    }else{
        char *buffer;
        char *stringtkn;
        printf("inserisci una stringa : ");
        scanf("%m[^\n]", &buffer);
        close(pipefd[0]);// chiuso canale il lettura che non mi serve
        write(pipefd[1], buffer , strlen(buffer)); // scrivo nella pipe
        close (pipefd[1]);
        free(buffer); // freee del bufferino
        wait(NULL); // vado un wait aspettando la terminazione del figlio
        exit(EXIT_SUCCESS);
    }
}
