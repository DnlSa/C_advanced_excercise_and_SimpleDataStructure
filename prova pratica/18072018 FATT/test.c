/*
SPECIFICATION TO BE IMPLEMENTED:
Implementare un programma che riceva in input tramite argv[] i pathname
associati ad N file, con N maggiore o uguale ad 1.

Per ognuno di questi file generi un processo che legga tutte le stringhe contenute in quel file
e le scriva in un'area di memoria condivisa con il processo padre. Si
supponga per semplicita' che lo spazio necessario a memorizzare le stringhe
di ognuno di tali file non ecceda 4KB.



Il processo padre dovra' attendere che tutti i figli abbiano scritto in
memoria il file a loro associato, e successivamente dovra' entrare in pausa
indefinita.


D'altro canto, ogni figlio dopo aver scritto il contenuto del file nell'area
di memoria condivisa con il padre entrera' in pausa indefinita.



L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo padre venga colpito da esso dovra'
stampare a terminale il contenuto corrente di tutte le aree di memoria
condivisa anche se queste non sono state completamente popolate dai processi
figli.
*/

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






#define MAX_ARG 10
#define MAX_SIZE 4096
#define SNAMEP "/parentsem" //il semaforo non ammette nomi con MAIUSCOLE

int j;
pid_t pid[MAX_ARG];
key_t key = 2828; // chiavi dell area di memoria
key_t key_old = 2828;
int k ;

void signal_handler(int signo){ /// passare argomenti qui detntro solo usando delle variabili globali
    int i, shm_id, byte_w;
    void *addr;

    // mi tocca chiudere i processi rimasti aperti altrimenti vanno tutti in concorrenza e aprono la stessa memoria condivisa piu volte stmapando i dati
    for(i=1; i<j;i++){
        kill(pid[i],SIGTERM);
    }

    i=0;
    while(key_old < key){
        printf("signal handler -> %d\n", key_old);
        if((shm_id = shmget(key_old, MAX_SIZE ,0))==-1){ // apre l area di memoria gia creata
            perror("errore to shmget memoria non ancora creata ");
            exit(-1);
        }
         key_old++; // incremento la chiave pe andare alla prossima area di memoria

        if((addr = shmat(shm_id ,NULL , 0 ))== (void *)-1){ // ne acquisisce l indirizzo
              perror("error to shmat");
              shmctl(shm_id , IPC_RMID ,0);
              shmdt(addr);
              exit(-1);
        }

        printf("la stringa del file %d -> \n%s \n",++i ,(char *)addr); // write prende dal buffer e la scrive su stdout
        fflush(stdout); // pulisco stdout

        shmctl(shm_id , IPC_RMID ,0); // rimuovo l area di memia
        shmdt(addr); // stacco indirizzo

    }
raise(SIGTERM); // termina il processo padre
}


int childProcess(int i , char * filename){
       printf("sono nel processo figlio \n");
      int fd , shm_id , byte_r, count=0;
      void *addr;
      fd = open(filename, O_RDONLY, 06660); // apre il file in sola lettura per scriverne il contenuto in un area di memoria condivisa con il par

      if((shm_id = shmget(key , MAX_SIZE , IPC_CREAT|0666 ))==-1){ //CREA LA MEMORIA CONDIVISA
           perror("errore to shmget");
           exit(-1);
     }

     if((addr=shmat(shm_id, NULL , 0 ))==(void *)-1){ // ne acquisice l indirizzo
         perror("error to shmat");
         shmctl(shm_id , IPC_RMID ,0);
         shmdt(addr);
         exit(-1);
    }
    char buff[MAX_SIZE];

    while ((byte_r = read(fd , (char *)addr, 1)) != 0){
        addr++;
        count+=byte_r;
    }// legge dal file e immagazzina nell area di memoria
    shmdt(addr); // alla fine faccio il detach dell
    close(fd); // chiudo il descrittore del file
    return 0; // uscita per tornare nel main
}


int main(int argc , char **argv){


    if(argc < 2 ){ // controllo dei parametri passati
        printf("usage : ./name_prog <pathname1> <pathname 2> ..... ");
        fflush(stdout);
        exit(-1);

    }

    void *status;
    int i;

    j=argc-1;
    sem_t *sem_id_PARENT = sem_open(SNAMEP , O_CREAT , 0666 , 0 ); // mi server per coordinare i processi(vedere se serve )
    //pid_t pid[j]; // tutti i processi figli che devono essere generati

    for(i=1; i<argc ; i++){

        if(!(pid[i] = fork())){
            printf("sono il figlio %d\n",i);
            childProcess(k, argv[i]);
            sem_post(sem_id_PARENT); // do il token al processo padre per poter continuare
            while(1){pause();} // attesa indefinita dei processi figli
        }


        else if(pid[k]== -1){
            perror("error to fail fork process");
            exit(-1);
        }

        else{ // il processo padre continuera la sua esecuzione in quanto sprovvisto di wait l unico che gestisce la sua esecuzione e la sem_wait
            sem_wait(sem_id_PARENT); // aspetto che il processo figlio finisca di scrivere nellarea di memoria condivisa
            //k++; // incremento contatore per il prossimo processo
            key++; // aumento la chiave per mappare una nuova area di memoria


        }
  }
printf("son il processo padre e attendo il segnale di terminazione\n");
signal (SIGINT, signal_handler);
while (1){pause();} // attesa indefinita processo padre
}
