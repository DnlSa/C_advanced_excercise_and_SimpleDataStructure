/*
SPECIFICATION TO BE IMPLEMENTED:


Implementare una programma che riceva in input, tramite argv[], il nomi
di N file (con N maggiore o uguale a 1).
Per ogni nome di file F_i ricevuto input dovra' essere attivato un nuovo thread T_i.



Il main thread dovra' leggere indefinitamente stringhe dallo standard-input 
e dovra' rendere ogni stringa letta disponibile ad uno solo degli altri N thread
secondo uno schema circolare.
Ciascun thread T_i a sua volta, per ogni stringa letta dal main thread e resa a lui disponibile, 
dovra' scriverla su una nuova linea del file F_i. 

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra' 
riversare su standard-output e su un apposito file chiamato "output-file" il 
contenuto di tutti i file F_i gestiti dall'applicazione 
ricostruendo esattamente la stessa sequenza di stringhe (ciascuna riportata su 
una linea diversa) che era stata immessa tramite lo standard-input.

In caso non vi sia immissione di dati sullo standard-input, l'applicazione dovra' utilizzare 
non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>




/*
 *

input -> ./prog primo.txt secondo.txt terzo.txt



pthread_mutex_init(&m1 ,NULL) ;
pthread_mutex_lock(&m1); -> prende il gettone
pthread_mutex_unlock(&m1); -> sblocca il gettone


*/




#define MAX_SIZE 4096
#define N 20
int n_arg;
char buff[MAX_SIZE];
int indice;

sem_t sem[N];
sem_t sem_main;



char *order[N];
int ordine[N];
int k;

void signal_handler(int signo){ // deve ripristinare la corretta
        int i;
        char bufferino[MAX_SIZE];
        int byte_r;
        for(i=0 ;i<n_arg ; i++){
                lseek(ordine[i], 0 , SEEK_SET); // imposto la session data al inizio
        }
        i=0;
        k=0;
        while ((byte_r =read(ordine[i], &bufferino[k], 1)) == 1 ){
                if(bufferino[k]=='\n'){
                        i++; // punto al prossimo file
                        bufferino[k]=' '; // invece del new line metto uno spazio
                        k++; // incremento bufferino  per il prossimo inserimento
                        if(i==n_arg) //qunando i = 4 allora torno a 0
                                i=0;
                }else
                        k++;
        }
printf("\n %s \n", bufferino);
exit(EXIT_SUCCESS);
}



void *thread_function(void *arg){

        char *file = (char *)arg;
        int fd;
        int byte_w ;
        if((fd= open(file, O_CREAT|O_TRUNC|O_RDWR , 06660))==-1){ // provoca l ordine diverso perhce genera latenza di apertura
                perror("errore nell apertura del file\n  ");
                exit(-1);
        }
        order[k]= file;
        ordine[k]= fd;
        k++;
while (1){
        printf("sono il thread CHILD del file %s \n" , file);
        sem_wait(&sem[indice]);
        printf("sono il thread CHILD sbloccato\n");
        if((byte_w = write(fd , buff, strlen(buff)))==-1){
                perror("errore nela scritutra ");
                exit(-1);
        }
        dprintf(fd , "\n"); // metto una new line;
        sem_post(&sem_main);
}

}






int main( int argc , char **argv){

   if(argc <2){
      printf("usage: input ./prog primo.txt secondo.txt terzo.txt\n ");
      fflush(stdout);
      exit(-1);
    }
    signal(SIGINT , signal_handler);
   n_arg = argc-1;
   pthread_t tid[n_arg];
   int ret;
   int i, j;
   void *status;

   sem_init(&sem_main , 0, 1);
   for( i =0 ; i<n_arg ; i++)
        sem_init(&sem[i], 0, 0);

  printf("di seguito l ordine dell anello \n");
   for( i =0,j=1 ; i<n_arg ; i++, j++){
        if((ret =pthread_create(&tid[i] , NULL , thread_function ,(void *) argv[j]))!=0){
                perror("errore pthread_create ");
                exit(ret);
        }
   }
   printf("\n");
   sleep(1);
   while (1){

        printf("sono il processo padre aspetto\n");
        sem_wait(&sem_main);
        printf("\nsono il processo padre sbloccato passo stringa: ");
        scanf("%[^\n]", buff ); // acquisisco la stringa
        getc(stdin);
        sem_post(&sem[indice]);

    }
}
