/*

SPECIFICATION TO BE IMPLEMENTED:

FATTO


Implementare una programma che ricevento in input tramite argv[] una stringa S
esegua le seguenti attivita'.

Il main thread dovra' attivare due nuovi thread, che indichiamo con T1 e T2.


Successivamente il main thread dovra' leggere indefinitamente caratteri dallo
standard input, a blocchi di 5 per volta, e dovra' rendere disponibili i byte
letti a T1 e T2.


Il thread T1 dovra' inserire di volta in volta i byte ricevuti dal main thread
in coda ad un file di nome S_diretto, che dovra' essere creato.
Il thread T2 dovra' inserirli invece nel file S_inverso, che dovra' anche esso
essere creato, scrivendoli ogni volta come byte iniziali del file (ovvero in testa al
file secondo uno schema a pila).



L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra'


Questa attivita' dovra' essere svolta attivando per ogni ricezione di segnale un apposito thread.

In caso non vi sia immissione di dati sullo standard input, l'applicazione dovra'
utilizzare non piu' del 5% della capacita' di lavoro della CPU.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

/*
 cos adobbiamo fare

 1) la stringa passata dovra essere spezzettata e passata a blocchi di 5 su un bufferino di 5 finche non la consumo tutta una volta arrivato alla fine verra attivata una scanf che legge dall stdinput
 2) mettere i semafori per far lavorare in oridine i thread



 */





// adotto sto metodo per assicurarmi di pulire stdin dopo aver letto tutti i byte che mi servono fflush(stdout) funziona bene ma fflush(stdin) ci tocca implemtarlo
#define fflush(stdin) while (getchar()!= '\n')
#define SIZE 5
#define MAX_SIZE 4096


// filename dei file da aprire
char *filename_1 = "S_diretto.txt";
char *filename_2 = "S_inverso.txt";

// prendono i descrittori
int fd_dir;
int fd_inv;

sem_t s1;
sem_t s2;
sem_t s3;
sem_t s4;
char pkt[5];

/*
 calcolare il numero dei byte che nei due file hanno la stessa posizione ma sono
tra loro diversi in termini di valore.

 */


void *thread_function_print(void *arg){
    long contatore =0;
    int i=0;
    int j;
    char buff_dir[MAX_SIZE];
    int byte_r_dir;
    char buff_inv[MAX_SIZE];
    char byte_r_inv;


    if((lseek(fd_dir ,0, SEEK_SET))==-1){
        perror("error to lseek");
        exit(-1);
     }
    if((lseek(fd_inv ,0, SEEK_SET))==-1){
        perror("error to lseek");
        exit(-1);
     }

    while ((byte_r_dir = read(fd_dir , &buff_dir[i], 1))==1){
            i++;
    }
    i=0;
     while ((byte_r_inv = read(fd_inv , &buff_inv[i], 1))==1){
            i++;
    }
    for(j=0; j<i; j++){
        if(buff_inv[j]==buff_dir[j]){
            contatore++;
        }else
            continue;
    }
    pthread_exit((void *)contatore);
}

void signal_handler(int sig){

    sem_wait(&s4);
    void *status;
    int ret;
    pthread_t tid_T3;
        if((ret = pthread_create(&tid_T3 , NULL , thread_function_print , NULL))!=0){
            perror("errore nella creazione del thread t3");
            exit(ret);
    }
    pthread_join(tid_T3 ,&status);
    long k = (long) status;
    printf("\ni caratteri nelle stesse posizioni che sono diversi sono %ld\n" , k );



close(fd_dir);
close(fd_inv);
exit(EXIT_SUCCESS);
}

void open_file(void){ // apertura dei file

    if((fd_dir=open(filename_1,  O_RDWR | O_CREAT |O_TRUNC , 06660 ))<0){
        perror("error to open file diretto");
        exit(-1);
    }
     if((fd_inv= open(filename_2,  O_RDWR | O_CREAT |O_TRUNC, 06660 ))<0){
        perror("error to open file diretto");
        exit(-1);
    }
}

void *thread_function_T1(void *arg){
redo:
    sem_wait(&s1);
    int byte_w;
    int count=0;
    byte_w = write(fd_dir , pkt , 5);
    sem_post(&s2);
    pthread_detach(pthread_self());
    printf("T1 esco\n");
goto redo;
    pthread_exit(0);
}


void *thread_function_T2(void *arg){
    off_t s;
    char buff[MAX_SIZE];
    int byte_r;
    size_t count;
redo:
    sem_wait(&s2);
     if((lseek(fd_inv ,0, SEEK_SET))==-1){
        perror("error to lseek");
        exit(-1);
     }
    int i=0;
    while ((byte_r = read(fd_inv , &buff[i] , 1))==1){
            count+=byte_r;
            i++;
    }
    printf("%s\n", buff);
     if((lseek(fd_inv ,0, SEEK_SET))==-1){
        perror("error to lseek");
        exit(-1);
     }
    write(fd_inv , pkt , 5); //la salva
    write(fd_inv , buff, count); //la salva
    printf("attualmente abbiamo %ld byte",count);
    sem_post(&s3);
    pthread_detach(pthread_self());
    printf("T2 esco\n");
    goto redo;

    pthread_exit(0);

}

int main(int argc , char **argv){

    signal(SIGINT , signal_handler);
    if(argc != 2){
        printf("errore .....");
        exit(-1);
    }
    if(atoi(argv[1])){
        printf("errore passa una stringa");
        exit(-1);
    }
    int i,j;
    pthread_t tid_T1;
    pthread_t tid_T2;

    open_file(); // apertura del file

    char buff[MAX_SIZE];
    //char *string;
    strcpy(buff , argv[1]); // copio nella stringa ;

    int ret;
    //int byte_r ;

    sem_init(&s1 , 0 , 0);
    sem_init(&s2 , 0 , 0);
    sem_init(&s3 , 0 , 1);
    sem_init(&s4 , 0 , 1);

    if((ret = pthread_create(&tid_T1 , NULL , thread_function_T1 , NULL))!=0){
            perror("errore nella creazione del thread T1");
            exit(ret);
    }


    if((ret = pthread_create(&tid_T2 , NULL , thread_function_T2 , NULL))!=0){
            perror("errore nella creazione del thread T2");
            exit(ret);
    }




while(1){
    sem_wait(&s3);
    printf("inserisci una stringa di 5 byte: ");
    for(i=0 ; i<5 ; i++){
        if((pkt[i]= getchar())=='\n'){
            pkt[i]=' ';
        }
    }
      if((lseek(fd_inv ,0, SEEK_SET))==-1){
        perror("error to lseek");
        exit(-1);
    }
    sem_post(&s1);
}



}
