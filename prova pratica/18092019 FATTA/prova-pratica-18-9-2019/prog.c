
/*
SPECIFICATION TO BE IMPLEMENTED:
Implementare una programma che riceva in input, tramite argv[], un insieme di
stringhe S_1 ..... S_n con n maggiore o uguale ad 1.


Per ogni stringa S_i dovra' essere attivato un thread T_i.


Il main thread dovra' leggere indefinitamente stringhe dallo standard-input.
Ogni stringa letta dovra' essere resa disponibile al thread T_1 che dovra'
eliminare dalla stringa ogni carattere presente in S_1, sostituendolo con il
carattere 'spazio'.



Successivamente T_1 rendera' la stringa modificata disponibile a T_2 che dovra'
eseguire la stessa operazione considerando i caratteri in S_2, e poi la passera'
a T_3 (che fara' la stessa operazione considerando i caratteri in S_3) e cosi'
via fino a T_n.
T_n, una volta completata la sua operazione sulla stringa ricevuta da T_n-1, dovra'
passare la stringa ad un ulteriore thread che chiameremo

OUTPUT il quale dovra'
stampare la stringa ricevuta su un file di output dal nome output.txt.





L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra'
stampare il contenuto corrente del file output.txt su standard-output.

In caso non vi sia immissione di dati sullo standard-input, l'applicazione
dovra' utilizzare non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/
/*
UNICA COSA CHE MANCA

Si noti che i thread lavorano secondo uno schema pipeline, sono ammesse quindi
operazioni concorrenti su differenti stringhe lette dal main thread dallo
standard-input.
*/




#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <sys/mman.h>



#define PAGE_SIZE 4096
#define N_PAGE 1
#define SNAMA "/sem"
#define SNAMB "/semap"
#define SIZE PAGE_SIZE*N_PAGE

/*
 ./prog agj polrt qwer

 */



static char *base_address;
sem_t *sem_id_a;
sem_t *sem_id_b;
char *filename = "output.txt";
int fd;
sem_t sem[max];


void signal_handler(int sig){
    char *stringa = "cat output.txt";
    printf("le stringhe inserite nell output.txt\n\n"); // padding a cazzo
    system(stringa);
    exit(0);
}


void *thread_function(void *arg){
    printf("sono nel figlio \n");
    int i, j;
    char *string = (char * )arg;
    int len_1 = strlen((char *)arg);
    char buffer_1[len_1]; // cosi posso spezzare in caratteri

    sem_wait(sem_id_b); // prende il gettone per lavorare
    int len_2 = strlen((char *)base_address); // lunghezza della stringa letta
    char *str2 = base_address;

    for(i=0; i<len_1 ; i++){
        for(j=0;j<len_2; j++){
            if( (*string) == (*str2) ){
                (*str2)= ' ';
                str2++;
            }else
                str2++;
        }
    string++;
    str2 = base_address;
    }
    pthread_exit(0);// uscita e vado al prossimo thread
}

// ultimo thread
void *thread_output(void *arg){


    printf("sto sitemando l out\n");
    int byte_w;
    int count=0;
     int len = strlen((char *)base_address);
     char *str = base_address;
     printf("stringa scritta in output.txt ->   %s\n ",str);
     byte_w = write(fd , base_address , len);
    dprintf(fd , "\n");

    pthread_exit(0);
}

void main(int argc , char **argv){

    if(argc <1){
        printf("udage : ./prog <string1>  <string 2> ....");
        fflush(stdout);
        exit(0);
    }

    if((fd = open(filename , O_RDWR | O_CREAT|O_TRUNC , 06660))==-1){ // il trunc serve a resettare il file
        perror("erorr to OPEN output.txt");
        exit(-1);
    }
    int n_arg = argc-1 , i;
    char *buff[n_arg];
    int ret;
    signal(SIGINT, signal_handler);
    sem_id_a = sem_open(SNAMA, O_CREAT, 0666 , 1 ); // parte con 1 perhce all inizio deve generare la stringa
    sem_id_b = sem_open(SNAMB, O_CREAT, 0666 , 0 ); // parte con 1 perhce all inizio deve generare la stringa

   base_address = (char *)mmap(NULL , SIZE , PROT_READ|PROT_WRITE , MAP_SHARED|MAP_ANONYMOUS, 0 , 0  ); // fatto una sola volta in quanto lavoro solo su questo
    memset(base_address , 0 , SIZE);
    pthread_t tid[n_arg]; // gli passo tutte le stringhe
    int res=1;

while (1){

      if(res){

        //sem_wait(sem_id_a); // prende il gettone per creare un astringa
       // memset(base_address, 0 , SIZE); // PULISCO L AREA DI MEMORIA
        printf("inserisci una stringa da modificare: ");
        scanf("%[^\n]", base_address); // inserisco la strringa nell area di memria
        getc(stdin);// brucio l ultimo carattere altirmenti alle iterazioni successive succede un casin

        printf("hai inserito : %s\n",base_address);

        for(i=0; i<n_arg ; i++){ // il thread main rida i gettoni a tutti i thread figli
            sem_post(sem_id_b);
        }
       res=0;
      }

    for(i=1; i<argc ; i++){
        if((ret = pthread_create(&tid[i], NULL , thread_function , (void *)argv[i] )) !=0){ // creazione del thread e passaggio dell arogmento nella thread_function
                perror("error to create thread");
                exit(ret);

        }pthread_join(tid[i], NULL); // partono in modo sequenziale
    }

    if((ret = pthread_create(&tid[0], NULL , thread_output , NULL )) !=0){ // creazione del thread e passaggio dell arogmento nella thread_function
                perror("error to create thread");
                exit(ret);
    }
    pthread_join(tid[0], NULL); // partono in modo sequenziale
    // voglio creare una memoria condivisa per tutti i thread
    res++;
}
}
