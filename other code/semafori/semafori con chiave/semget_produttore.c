#include "header.h"


int produttore(void *addr , int sem_ds){

    struct sembuf sops[1];

    sops[0].sem_num = 1 ; // questo e l identificatore del semaforo
    sops[0].sem_op  = -1 ; // quando e minore di 0 preleva il token
    sops[0].sem_flg = 0 ; // non ci sono particolari flag da passargli

    if (semop(sem_ds, sops, 1) == -1) {
        perror("SEMOP error in consumer");
        exit(EXIT_FAILURE);
    }
        // ciclo di operazioni di sorta
    printf("inserisci una stringa che sara letta dal consumatore: ");
    scanf("%[^\n]",(char *)addr);

    //*((char *)addr) = '\0'; // lo mette nell stdin
    getc(stdin); // lo alloca in stringa

    sops[0].sem_num = 0 ;// il semaforo 0
    sops[0].sem_op  = 1 ;// gli do il gettone per leggere la mia stringa
    sops[0].sem_flg = 0 ;// non ho flag(puo essere IPC_NOWAIT o SEM_UNDO)

    if (semop(sem_ds, sops, 1) == -1) { // IMPOSTA
        perror("SEMOP error in consumer");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int main(void ){

key_t shm_key = 5155;
key_t sem_key = 2424;
int num_semapfores= 2;
int ret;


///1) creiamo prima di tutto la memroia condivisa

    // chiave , grandezza , permessi
int shm_ds = shmget(shm_key ,TOT_SIZE , 0 ); // (MODIFICATA )perche e stata gia instanziata dal cons.
    if(shm_ds == -1){
        perror("SHMGET error in MAIN consumer");
        exit(-1);
    }

void *addr = shmat(shm_ds, NULL , 0); // descrittore , indirizzo scelto dal so , nessuna opzione
    if(addr == (void *)-1){
        perror("SHMAT error in MAIN consumer");
        shmctl(shm_ds , IPC_RMID, 0);
        exit(-1);
    }

////2) creiamo il semaforo

int sem_ds = semget(sem_key ,num_semapfores,0); // RIPRENDE LA CHIAVE DEL CONSUMATORE CHE E STATA GIA CREATA

if(sem_ds == -1){
    perror("SEMGET error in MAIN consumer");
    shmctl(shm_ds , IPC_RMID, 0);
    shmdt(addr);
    exit(-1);
}

////// 3) ciclo operazioni
do{
    ret = produttore(addr , sem_ds); /// fa qualcosa
} while(ret);


////// 4) rimozioni

//shmctl(shm_ds , IPC_RMID, 0); // rimozione(DEVE ESSERE CHIAMATA SOLO DA COLUI CHE INSTANZIA )
shmdt(addr); // detach


//semctl(sem_ds , IMP_RMID, 0) // rimozione(DEVE ESSERE CHIAMATA SOLO DA COLUI CHE ISTANZIA)

return 0 ;

}
