 
/*
(non capisco la gestione segnali fra A e B)

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo A venga colpito esso dovra'
inviare la stessa segnalazione verso il processo B.

Se invece ad essere
colpito e' il processo B, questo dovra' riversare su standard output il
contenuto corrente del file F.



*/

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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
/*
1)  problema con shmget -> risolto lasciando solamente smget(key , SIZE , IPC_CREAT| 0666) ho tolto IPC_EXCL -> che vede se esiste e mi da errore

2)  sincronizzazione risolta con i seguenti passaggi

#define SNAME "/semaforo"

MAIN ->  sem_t *sem_id = sem_open(SNAME , O_CREAT, 0666, 0 );

CHILD -> sem_wait(sem_id) -> e bloccato finche non ci sara un post che gli dara il gettone

PARENT -> alla fine ho messo sem_post(sem_id)

3)  e vedere se funziona la scrittura su file (NON FUNGE) -> RISOLTO passando al thread il descrittore del file


4) lettura delle stesse aree di memoria -> il primo processo prende la chiave e crea tutte memorie condivise incrementando la chiave di 1 ogni volta
                                        -> il processo figlio riparte dalla prima chiave e va avanti

5) il carattere new line e stato risolto con una dprintf che permette di inserire un carattere in coda a quando scritto dprintf(fd ,"%c" ,'\n');


6) I WARNING della pthread_create sull argomento  sono stati fixati dichiarando direttamente long le variabili e poi ricastandole al tipo esatto

7) nei thread iniziali dopo l inserimento e NECESSARIO PASSARE DOPO getc(stdin); cosi da consumare quanto scritto dentro altrimenti salta l istruzioni successive

// infine gestisco il segnale sigINT
*/
#define SIZE 4096
#define SNAME "/semaforo"
pid_t pid;
key_t key_A = 2929;
key_t key_B = 2929;
long fd;
char *filename;

void signal_handler_A(int sig){
    printf("sono il processo padre %d \n",sig );
    kill(pid, SIGINT);
    raise(SIGTERM);
}

void signal_handler_B(int sig){
    char buff[SIZE];
    sprintf(buff , "cat %s\n",filename );
    system(buff); // questa esegue il comando cat filename.
    printf("sono il processo figlio %d \n", sig);
    close(fd);
    raise(SIGTERM);
}

void *thread_funcion_B(void *arg){
    long fd = (long )arg;
    int shm_id;
    void *addr;
    int bytes_w;

    if((shm_id = shmget(key_B , SIZE , 0))==-1){ // la memoria condivisa esiste
         perror("error to SHMGET child ");
         exit(EXIT_FAILURE);
    }
    if((addr=shmat(shm_id , NULL , 0)) == (void *)-1){ // ne acquisisco l indirizzo attaccandola al mio processo
        perror("error to SHMAT");
        shmctl(shm_id , IPC_RMID , NULL);
        shmdt(addr);
        exit(EXIT_FAILURE);
    }
    int len = strlen((char *)addr);

    bytes_w = write(fd , (char *)addr , len); // ne faccio la scrittura nel file
    dprintf(fd ,"%c" ,'\n');

    printf ("sono il thread_B ho scritto %d \n", bytes_w );

    shmctl(shm_id , IPC_RMID , NULL); // rimuovo la vecchia memoria condivisa
    shmdt(addr);
    pthread_exit(0);
}

 void *thread_funcion_A(void *arg){ // sono i thread scrittori

     long i;
     i = (long)arg;
     int shm_id;
     void *addr;

     if((shm_id = shmget(key_A , SIZE , IPC_CREAT|0666 ))==-1 ){ // creazione del area di memoria
         perror("error to SHMGET parent");
         exit(EXIT_FAILURE);
    }
    if((addr=shmat(shm_id , NULL , 0)) == (void *)-1){ // la attacco al mio processo
        perror("error to SHMAT");
        shmctl(shm_id , IPC_RMID , 0);
        shmdt(addr);
        exit(EXIT_FAILURE);
    }
    printf("sono il thread_A %ld ",i);
    printf("inserisci una stringa: ");
    scanf("%[^\n]",(char *)addr); // scrivo nella memoria condivisa
    getc(stdin); // serve perche altrimenti rimane in stdin la stringa presa(perche non e stampata)
    shmdt(addr); // la stacco dal processo
    pthread_exit(0);
}



int main(int argc , char **argv){

    if(argc != 3){
        printf("usage: ./prog_name <filename> < N >\n");
        exit(-1);
    }
    filename = argv[1];
    int n = atoi(argv[2]);
    int  ret;
    long i,j ;

    // per verificare che sia veramente una strigna il primo argomento verra passato con atoi se restituisce un ERRORE allora abbiamo una stirnga altrimenti e stato passato un intero
    if(n<1 || (fd = atoi(argv[1])) != 0){
        printf("iserisci n > 1  come secondo argomento e una stringa come primo \n");
        exit(-1);
    }

    sem_t *sem_id = sem_open(SNAME , O_CREAT, 0666, 0 ); // semaforo che blocca il processo figlio aspettando che i scrittori compilino le aree di memoria preposte

    pthread_t tid_A;
    pthread_t tid_B;



    pid = fork();

    if(pid == 0){ // PROCESSO FIGLIO

        signal(SIGINT , signal_handler_B); // questa seignal fa parte solamente del processo B

        sem_wait(sem_id); // va in aspetto che tutti i thread del parent compilino la loro area di memoria predefinita
        fd = open(filename , O_CREAT|O_TRUNC|O_RDWR ,06660);
        if(fd == -1){
            perror("error to OPEN file in MAIN");
            exit(EXIT_FAILURE);
        }

        for(i=0;i<n ; i++ ){ // spawn dei thread scrtittori su file
           ret= pthread_create(&tid_B , NULL , thread_funcion_B , (void *)fd);
           if(ret != 0){ // controllo dell errore della create
                perror("erorr in pthread_create");
                exit(ret);
           }
           else{
             pthread_join(tid_B, NULL); // QUANDO SI SPAWNANO I THREAD FIGLI I ATTENZIONE CHE UNA SOLA VARIABILE STATUS PORTA A CHIUDERE TUTTO SUBITO SENZA LO SPAWN DDEI PROCESSI SUCCESSIVI
             key_B++; // aumento la key cosi ad ogni passaggio prendo la memoria instanziata
           }
        }
       exit(0);

    }else if(pid == -1){
        perror("error fork process ");
        exit(-1);
    }
    else{  // PROCESSO PADRE


        for(j=0;j<n ; j++ ){
           ret= pthread_create(&tid_A , NULL , thread_funcion_A , (void *)j); // crei thread scrittori
           if(ret != 0){
             perror("erorr to create thread");
             exit(ret);
            }
           else{
             pthread_join(tid_A, NULL); // aspetto che abbiano scritto su memoria condivisa
             key_A++; // inclremento la chiave per il prossimo thread
           }
        }sem_post(sem_id);
        wait(NULL); // ritorno del processo figlio
    }
printf("\nsono nel processo padre attedo il segnale di SIGINT \n");
signal(SIGINT , signal_handler_A); // questa signal fa parte solo del processo A (NON METTERE PRIMA DELA FORK ALTRIMENTI ANCHE IL PROCESSO FIGLIO LA EREDITA )
while (1){pause();}
}
