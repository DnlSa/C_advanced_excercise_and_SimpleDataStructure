#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define TOT_SIZE 4096

key_t shm_key = 5155;
key_t sem_key = 2424;




int consumatore(void *addr , int sem_ds){

    struct sembuf sops[1]; // dichiaro la struttura per gestire il semaforo(COLLEGATA A QUANTO INSTANZIATO GIU)

    sops[0].sem_num = 0 ; // questo e l identificatore del semaforo
    sops[0].sem_op  = -1 ; // quando e minore di 0 preleva il token
    sops[0].sem_flg = 0 ; // non ci sono particolari flag da passargli

    if (semop(sem_ds, sops, 1) == -1) {
        perror("SEMOP error in consumer");
        exit(EXIT_FAILURE);
    }
    printf("il produttore scrive :%s\n", (char *)addr);

    sops[0].sem_num = 1 ;// il semaforo 1
    sops[0].sem_op  = 1 ;// gli do il gettone
    sops[0].sem_flg = 0 ;// non ho flag(puo essere IPC_NOWAIT o SEM_UNDO)

    if (semop(sem_ds, sops, 1) == -1) { // IMPOSTA
        perror("SEMOP error in consumer");
        exit(EXIT_FAILURE);
    }
    return 1;
}


int main(void ){

int shm_ds;
void *addr;
int sem_ds;
///1) creiamo prima di tutto la memroia condivisa

    // chiave , grandezza , permessi
shm_ds = shmget(shm_key ,TOT_SIZE , IPC_CREAT|0666 );
    if(shm_ds == -1){
        perror("SHMGET error in MAIN");
        exit(-1);
    }

addr = shmat(shm_ds, NULL , 0); // descrittore , indirizzo scelto dal so , nessuna opzione
    if(addr == (void *)-1){
        perror("SHMAT error in MAIN consumer");
        shmctl(shm_ds , IPC_RMID, NULL);
        exit(-1);
    }

////2) creiamo il semaforo

int num_semapfores= 2; // NUMERO DI SEMAFORI CHE VOGLIO


sem_ds = semget(sem_key ,num_semapfores, IPC_CREAT|0666); // INIZIALIZZAZIONE 1) numero di chiave 2) numero di semafori 3) permessi

if(sem_ds == -1){
    perror("SEMGET error in MAIN consumer");
    shmctl(shm_ds , IPC_RMID, 0);
    shmdt(addr);
    exit(-1);
}

int ret = semctl(sem_ds,0,SETVAL, 0); // IMPOSTAZIONE
    if(ret==-1){
        perror("SEMCTL error in MAIN semaphore 0");
        semctl(sem_ds ,0,IPC_RMID ); // in caso di errore viene distrutto il semaforo
        shmctl(shm_ds , IPC_RMID , NULL);
        shmdt(addr);
        exit(EXIT_FAILURE);

    }

ret = semctl(sem_ds,1,SETVAL, 1); // verra impostato a 1
     if(ret==-1){
        perror("SEMCTL error in MAIN semphore 1");
        semctl(sem_ds ,1 ,IPC_RMID);
        shmctl(shm_ds , IPC_RMID , NULL);
        shmdt(addr);
        exit(EXIT_FAILURE);
    }



////// 3) ciclo operazioni
do{
    ret = consumatore(addr , sem_ds); /// fa qualcosa
} while(ret);


////// 4) rimozioni
semctl(sem_ds , IPC_RMID, 0); // rimozione semaforo
shmctl(shm_ds , IPC_RMID, 0); // rimozione memoria condivisa
shmdt(addr); // detach


return 0 ;

}
