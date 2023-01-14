LISTA DELLE API VISTE FINO ADESSO

1) scanf("%[^\n]", buffer  ); -> prende in input una stringa fino al new line

1.1) scanf("%m[^\n]" , buffer); -> prende stringa  fino alla new line  in piu ha che memorizza la stringa facendo una malloc e quindi inserendola nello stack anziche nel campo data (RICORDARE CHE ALLA FINE
                                 TOCCA FARE SEMPRE LA FREEE DEL BUFFER PERCHE ALTRIMENTI CON VALGRIND ESCE CHE HAI DELLA MEMORIA ALLOCATA NON LIBERATA)

1.2) scanf_s ("%s", buffer , 30 ) -> variante della calssica scanf ma piu sicura in quanto oltre al tipo che accetta tocca specificare anche la taglie che si deve acquisire in input

2) IL fflush(stdout) -> importantissimo mettendo subito dopo una printf dopo la stampa da terminale della stringa l stdout ferra flushato a 0 e pulito . molto importante perche in cicli while dove non
                        vengono stampati i valori rimangono in questo file di  sistema e quando si richiama la scanf avendo ancora l stdout pieno verra saltata l operazione

   fflush(stdin) -> abbastanza inutile


3) getchar() -> prende un caratttere e lo mette in out se non lo trova sull stdout allora lo chiede al sistema operativo tramite un interrupt e va in attesa che inserisoc qualcosa da tastiera

   gets() -> acquisisce una stringa in input fino al carattere \n il prblema e che e molto pericolosa e puo creare dei bug latenti


////////////////////////////////////   PROCESSI  ////////////////////////


4)  pid fork(void)  -> crea un clone del processo chiamante  non accettaargomenti ma solamente ha la restituzione
//                         pid del processo appena creato al processo padre
//                         0 e restituito al processo figlio .


5) wait(&status)  -> si dichiara una variabile status la cui area di memoria verra sovrascritta dalla exit() del processo che sta terminando .
//                     con printf("%d", status >> 8) ; stmapiamo il codice di terminazione della exit in quando vengono scriti gli 8 bit meno significativi della variabile status
//                     status e possibile trovarla come
//                     int * status  , int status   e pressoche uguale.

6) void exit(int n) -> indica generalmente il codice di terminazione del processo e come i return ma con i processi e altre robe piu avanzate scrivere return e abbastanza ambiguo e sbagliato
      //             i flag ce ne sono moltissimi e sono tutti scritti da qualche parte i piu usati sono
//                   0 -> terminazione corretta
      //             (VEDERLI)

7) pid_t waitpid(pid_t pid , int *status , int options) -> invoca l attesa di un determinato processo passato nella varibile pid

8) pid_t getpid(void) -> resituisce il PID del processo che lo ha chiamato

9) pid_t getppid(void) -> restituisce il PID del processo padre del processo figlio  chiamante


/////////////////////////////////// EXEC //////////////////////////////////////////
NOTA :
l	--------------------------->      Gli argomenti a riga di comando sono passati individualmente alla funzione.

v	--------------------------->       Gli argomenti a riga di comando saranno passati alla funzione tramite un array di puntatori.


10)  int execve(const char *pathname, char *const argv[], char *const envp[]);     // VERA E UNICA SYSTEMCALL per la run di applicativi .

11) int execl(const char *pathname, const char *arg, ... /* (char  *) NULL */); // VARIANTE PATH

//lista arogmenti dell API ->  1) /bin/ls   2) argomenti addizionali come il nome dell aplicativo da cercare e magari anche qualche flag  "ls" , "-l".. 3) NULL oppure 0 ;
 /*execl("/bin/ls","ls", "-l", (char *)0);*/


12) int execlp(const char *file, const char *arg, .../* (char  *) NULL */); // VARIANTE FILE  accetta  "ls" , "cd "
// 1) passo il nome dell applicativo  2) come argomenti passare di nuovo il nome dell apllicativo piu i vari flag  3) terminatore NULL oppure 0 ;  /*execlp("ls","ls", "-l", (char *)0);*/


13) int execle(const char *pathname, const char *arg, ... /*, (char *) NULL, char *const envp[] */); // VARIANTE PATH accetta "/bin/ls" MA CHE ACCETTA ANCHE VARIABILI D AMBIENTE


14) int execv(const char *pathname, char *const argv[]); // VARIANTE PATH   accetta "/bin/ls" , ecc

15) int execvp(const char *file, char *const argv[]); // VARIANTE FILE  accetta  "ls" , "cd "

16) int execvpe(const char *file, char *const argv[], char *const envp[]); // // VARIANTE PATH accetta "/bin/ls" MA CHE ACCETTA ANCHE VARIABILI D AMBIENTE


/////////////////////////// VARIABILI D'AMBIENTE //////////////////////////////////////////

17) char *getenv(char *name ) -> richiede un puntatore alla varibile d ambiente

18) char putenv(char *string) -> cambia il valore di una variabile d ambiente .
       //                        PARTICOLARITA' -> PATH = /usr/bin : /bin : /home -> si adottano 2 punti per inserire multi valori nella variabile PATH

19) int setenv(char *name , char *valore , int overwrite) -> crea una variabile d ambiente impstando nome ,  valore n  , e flag di sovrascrittura casomai gia esistesse


20) int unsetenv(char *name) -> invoca l eliminazione di una variabile d ambiente


//////////////////////////////////////////////   THREAD /////////////////////////////////////

NOTE :
per creare i thread tocca includere prima di tutto
#include <pthread.h>
inoltre nella compilazione del programma tocca specificare il flag   ->    -lpthread


21) int prhread_create(pthread_t tid , pthread_attr_t *attr , void *(*function)(void *) , void *arg ) -> incova la creazione di un trhread
//                        1)buffer info del thread  , 2) puntatore al buffer degli attributi  , 3) puntatore alla funzione che deve RUNNARE  , 4) puntatore al buffer argomenti che tocca passare alla funzione

22) void pthread_exit(void *status) -> puntatore che mi dice il codice di uscita (puo essere un puntatore ad una zona di memoria con dati elaborati oppure mi puo restituire anche un int  , ecc);

23) int pthread_join(pthread_t tid , void **status) -> attende che finisca il thread figlio o un thread specifico
    // 1) thread che tocca aspettare,  2) doppio puntatore che contiene il codice di terminazione del thread.


24) pthread_t prhead_self(void) -> serve solamente a restituire l identificatore del thread chiamante

25) pthread_detach() -> il thread che chiamera questa finzione una volta termianta la sua esecuzione verra rilasciata completamente la memoria che lui stesso ha usato


26 ) si richiama la stessa API del punto 6

        void exit(int n) -> termina il solo thread che la chiama
        void exit_group(int n) -> termina tutti i thread e uccide anche il processo


 NOTE :speighiamo un ulteriore API importante per l uso dei thread e la loro sincronizzazione
  L idea di questa API e di creare delle barrirere per cercare di sincronizzare i thread durante l esecuzione del codice .
  ES.
  il thread main deve assemblare una matrice e il calcolo e ripartito fra piu thread (perche matrice sono molto grandi) inserendo le barrirere possiamo rendere disponibili
  i dati nell tempistiche giuste per far prenderli dal thread main e inserirli nella amtrice

 variabile -> pthread_barrier_t barrier ; -> variabile barrirera


26.1) int pthread_barrier_init(pthread_barrier_t *restrict barrier, const pthread_barrierattr_t *restrict attr, unsigned count); -> inizializza la barriera deve essere passata per PRIMA
      1) barrier -> variabile barrieraa
      2) attributi della barriera NULL (default )
      3)  count specifica il numero di thread che devono chiamare pthread_barrier_wait () prima che uno di essi restituisca correttamente dalla chiamata. Il valore specificato da conteggio deve essere maggiore di zero.

// va in fallimento se :
      1) Il sistema non dispone delle risorse necessarie per inizializzare un'altra barriera.
      2) Il valore specificato da conteggio è uguale a zero.
      3) Esiste memoria insufficiente per inizializzare la barriera.

26.2) int pthread_barrier_wait(pthread_barrier_t *barrier) -> punto dove tutti i thread si fermeranno quando sara dichiarata .  quando tutti i thread(specificati da count ) saranno arrivati su una barriera verranno fatti proseguire


26.3) int pthread_barrier_destroy(pthread_barrier_t *barriera ); -> si invoca la distruzione della barrirea
      // genera errori nei seguenti casi
      1) la barrirera e usata da un altro thread
      2)valore della barrira e sbagliato



//////////////////////////////////PRIOTIRTA SU SHCEDULING ////////////////////////////////////////////////////////////////

NOTA :

cosa passo a WHICH??

PRIO_PROCESS -> indica un processo specifico
PRIO_PGRP -> indica un gruppo di processi
PRIO_USER -> indica un processo di un utente


cosa passo a WHO?
il pid del processo
il gruppo di processi per pid
i preocessi user sempre per pid


27) int nice(int incremento ) -> incrementa la nice del processo su cui e runnata la funzione

28) int getpriority(int which , int who) -> prende in input il processo a cuoi devo leggere l indice di priorita

29) int setpriority( int which , int who , int priority) -> questo cambia l indice di priorirta del processo


30) int sched_setscheduler(pid_t pid , int policy , const struct sched_param *p ); -> imposta lo scheduler del THREAD

//   1) pid del thread  , 2) politica adottata dal thread , 3) puntatore alla struttura che definisce lo scheduler dei miei thread

31) int sched_getscheduler(pid_t pid); -> crea una query(richiesta ) di prendere le info dalla struttura di definizione dello scheduler dei miei thread


/////////////////////////////////AFFINITA DEI THREAD SU LE CPU ///////////////////////////////////////////


QUESTE API FUNZIONANO GENERALMENTE SOLO PER PROCESSI

32) int sched_setaffinity(pid_t pid , size_t cpusetsize , const cpu_set_t *masck); -> imposta l affinita di processore sul processo

//          1) inserisco il pid del thread , 2) imposto la grandezza della maschera di bit  , 3) passo la maschera di bit  [1,0,0,1,1,0] maschera dove 0 -> cpu non esegue 1-> la cpu esegue

33) int sched_getaffinity(pid_t pid , size_t cpusetsize , const cpu_set_t *masck); -> chiede le info per la set affinity dei thread . per vedere la maschera di bit

DI SEGUITO IMPOSTO VARIANTI PER I THREAD

32.1)   int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,const cpu_set_t) *cpuset); -> imposta affinita di processore sul thread che ha chiamato l API
33.1 )      int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, cpu_set_t *cpuset); -> chiede le info per la set affinity del thread che ha chiamato l API

NOTA VARIANTE VIA DI MEZZO  -> sched_setaffinity(gettid(), size_t cpusetsize , const cpu_set_t *masck); -> Passo all api 32 il gettid() come primo argometno

/////////////////////////////// PERMSSI SU FILE  //////////////////////////////////

// permessi dei fiel OGNI utente del sistema ha un codice numerico associato (anche i gruppi lo hanno) esempio danilo: 1000

34) uid_t getuid() -> torna lo user ID dell utente che ha lanciatto il processo

35) int  setuid(uid_t uid ) -> prende in input l uid dell utente e ne setta uno personalizzato generalmente solo l utente root puo utilizzarlo // VEDERE BENE PERCHE NON HO CAPITO BENE COSA DEBBA FARE


/////////////////////////////// OPERAZIONI SU FILE ////////////////////////////////////////

/*          NOTA
  Se va tutto bene la crazione del file restituisce il suo descrittore per poterci accedere
  ALTRIMENTI -1  se fallisce

  CAMPO FLAGS DELL OPEN E POSSIBILE SPECIFICARE

  O_RONLY -> file in sola lettura
  O_WONLY -> file in sola scrittura
  O_RDWR -> file in lettura e scrittura
  O_APPEND -> si posizione alla fine del file e inserisce direttamente le robe
  O_CREAT  -> creazione del file in caso non esista
  O_TRUNC -> trunc significa che elimino il contenuto se esiste
  O_EXECL -> (esclusivo garantisce che il file e effettivamente creato dalla chiamata corrente )


 */


36) int creat(char *filename , mode_t mode ) -> invoco la creazione di un file passandogli  il nome del file da creare ed i suoi permessi di accesso

36) int open(char *filename , int flags , mode_t mode ) -> invoco l apertura di un file e se non esiste lo crea
      // 1) strigna nome file  , 2) flag per l apertura es. O_CREAT crea il file  , 3) spercifica la modalita di accesso al file

37) int close(int descriptor) -> chiudo il file in precedenza aperto


38) ssize_t read(int descriptor , char *buffer , size_t size) -> invoco  la lettura del file e trasporta tutto nel buffer allocato appositmente

39) ssize_t write(int descriptor , char *buffer , size_t size) -> invoco la scrittura del file va prende come sorgente i dati del buffer e scrive sieze byte nel file

//SOLO PWER  la 38 e 39
//1)  descrittore del file che voglio leggere , 2) buffer di destinazione del contenuto del file  , 3) taglia un byte di quanto vogliamo leggere
// RITORNO  -1 FALLIMENTO  ,   numero di byte scritti in caso di SUCCESSO


40) off_t lseek (int fd , off_t offset , int whence) -> specifica dopo l apertura dopo aver aperto il file da dove devo partire inizio da un offset specifico o dalla fine

      //1) canale associato alla sessione , 2) di quanto voglio spostare l indice  , 3) indice da dove devo spostarmi 0-> iniozio  , 1 -> posizione corrente , 2-> dalla fine
      //attenzione non fare mai le scritture inconsistenti del tipo lseek(fd , -10 , 0 ) -> da errore perche non posso spiazzarmi di -10 byte dall inizio del file

41) int dup(int descriptor) -> invoca la duplicazione del descrittore che gli passo l altro sara collegato all originale se modifico il nuovo modifico il vecchio

      /*
       molto utile perche posso modificare la tabella dei file descriptor la particolarita dell api e che i descrittore verra posizionato alla prima enty libera

       ESEMPIO so che
       0 -> stdin
       1 -> stdout
       2 -> stderr

       se chgiamo close(0) -> chiudero il file stdin e la entry della tabella per process rimarra libera
       poi chiamero una dup(descriptor che voglio io ) -> questo descrittore verra copiato al posto del stdin
       se in seguito alcune procedure chiamano stdin queste andranno a lavorare su questo file anziche sul vero stdin che e stato chiuso
       ROBA ADOTTATA NELLE PIPE
       */

42) int dup2(int descriptor , int NUOVO descrittore ) -> stessa cosa della dup ma specifico io il nuovo descrittore

/*

Il vecchio e nuovo file descriptor possono essere utilizzati interscambiabilmente. Essi condividono locks, puntatiori di file position e flag, ad eccezione del flag close-on-exec.
Per esempio e' possibile effettuare una lseek() su un file descriptor e ritorvarsi la posizione modificata su entrambi i file descriptor.

Flag close on exec -> indica se il descrittore deve essere chiuso automaticamente all'invocazione di una system call della famiglia exec().

VALORE DI RITORNO
Le funzioni dup() e dup2() ritornano il nuovo file descriptor, oppure -1 in caso di errore.
 */

43) int fcntl(int fd  , int cmd , arg....) -> esegue un operazione specificata da CMD sul file descriptor specificato in piu e possibile all occorrenza passare eventuali argomenti
/*
 comandi dipsonibili

F_DUPFD -> fa la stessa cosa di dup2 in caso di successo mi viene restituito il nuovo file
           descriptor


F_GETFD -> legge il flag close-on-exec
            se il bit FD_CLOEXEC e a 0 , il file puo rimanere aperto con la chiamata ad exec() , altrimetni verra chiuso

F_SETFD -> imposta il flag close-on-exec specificandolo della parte ARG dell API

F_GETFL -> legge i flags del file descriptor
           ritornano i flags con cui e stato settato l apertura del file dalla funzione open()

F_SETFL -> imposta i flags del file descriptor al valore speficifato da arg
            Solamente O_APEEND , O_NONBLOCK , O_ASYNC possono essere impostati
_______________________________________________________________________________

vedasi file fcntl-command

struttura diu lock (usata di solito per bloccare accessi a file se usati da uno specifico thread o processo )

    struct flock fl = {};  // strututa definita nell file header fcntl.h

    fl.l_type = F_WRLCK; // blocco della lettura e scrittura
            // specifica il tipo di LOCK che voglio
            1) F_RDLCK -> blocco in lettura
            2) F_WRLCK -> blocco in scrittura
            3) F_UNLCK -> sblocca


    fl.l_whence = SEEK_SET; //muove il puntatore all inizio del file
            corrisponde all argomenti usati dalla fseek o l seek definisce principalemente l offset nel file specifico
            1) SEEK_SET -> file pointer della sessione dati puntata in testa
            2) SEEK_CUR -> file poiuntr punta nel mezzo del file
            3) SEEK_END -> file pinter punta alla fine del file

    fl.l_start = 0;
            definisce l offset di inizio della regione dove il lock e applicato
            tale offset e fornito in byte rispetto al punto specificato dall l_whence

    fl.l_len = 0;
            specifica la lunghezza della regione da bloccare . UN VALORE 0 e traatato come carattere speciale e significa e estende il blocco fino alla fine del file



      fcntl(fd, F_SETLKW, &fl)
          con la funzione fcntl imposto il blocco sul file puntando il file descriptor do il comando di set lock e infine passo l indirizzo della struttura che contiene le info

continua nel file BLOCCHI FILE
 */

44) int link (const char *oldpath , const char *newpath) -> creo un HARDLINK ulteriore al vecchio path

45) int unlink(const char *pathname ) -> cancella l hardlink specificato

/*
 inseriscono ed eliminano delle entry nelle tabelle delle directory

 La funzione link() ritorna 0 in caso di successo e -1 in caso di errore.

 ls -lai  (comando da terminale unix )

 INDICE |   PERMESSI | NUMERO DI HARDLINK | UTENTE         | NUERO DI BYTE  | ULTIMA MODIFICA  | filename
  556     drwxr-xr-x         1              danilo danilo       604           mag 13 12:51       Scrivania

 */

46) int symlink(const char *oldpath , const char *newpath) -> creo una stringa che punta ad un link piu complesso insomma crea un nuovo nome per il file
/*

 ATTENZIONE :
 1) Nessun controllo e' effettuato per oldpath.
    Nel caso di impiego di symbolic link, e' possibile cancellare i dati del file oldpath quando si tenta di eliminare l'entry dal file system qualora non esistano altri hard links

 2) Se newpath e' gia' esistente, symlink() fallisce.

 3) La funzione symlink() ritorna 0 in caso di successo e -1 in caso di fallimento.

 */

/////////////////// API di creazione delle cartelle //////////////////////


47) int mkdir(const char *pathname , mode_t mode) -> crea la cartella

// 1) dove devo creare la cartella  , 2) modalita di accesso alla cartella

48) int rmdir(const char *pathname) -> invoca la rimozione di una cartella


///////////////////////////PERMESSI DI ACCESSO /////////////////////

49) int chmod(const char*path , mode_t mode) -> imposta i permessi di accesso ad un file

// 1) speficifico il nome di un file  2) ne specifico la modalita di accesso (tocca farlo in codifica ottale eeeh ) es 666 o 777

50) int fchmod(int fd  , mode_t mode) -> imposta i permessi ai file solo se un file e aperto

// 1) accetta il file descriptor , 2) ne specifico i permessi per l accesso (fatto sempre in codifica ottale);

///////////////////////////////////////////////////////////////////

51) int stat(const char *filename , struct stat *buff) -> spefico un puntatore a struttura dove verranno scaricate tutte  le informazioni riguardanti al file puntato da filename
// 1) filename del file ,  2) puntatore alla struttura dove verranno scaricate le info

52) int fstat(int fd , struct stat *buff) -> stessa cosa di su ma funziona solamente quando e aperto il file e ho il suo descrittore per arrivarci
// stessa cosa sole che invece del file name ne passo il descrittore in quanto file aperto


//////////////////////SINCRONIZZAZIONE DATI /////////////////////////////////////////////

le seguenti API possono risultare piu lente in quanto si invoca l interazione con un dispositivo esterno per la sincronizzazione

53) int fsync(int fd); -> sincronizza dati tra BUFFER CACHE e DISPOSITIVO
      (FLASHO tutto )

54) int fdatasync(int fd) -> sincronicca un file completo caricato nel buffer cache con il disco (FLUSHA solamente i dati utente )



/////////////////////////////////////////////////////////////////////////////////////ù


55) int strcmp(const char *s1 , const char *s2 ) -> comparo 2 stringhe

      0 -> se le 2 stringhe sono uguali
      NEGATIVO  -> se il valore di s1 e MINORE di s2
      POSITIVO  -> se il valore di s1 MAGGIORE di s2

56) sleep(int number) -> blocca il processo o il thread che l ha chiamata. number sono i secondi di wait che il processo eseguirà

57) perror(const car *string) -> definisce un messaggio d errore scritto nello standard error

//////////////////////PIPE /////////////////////////////

NOTA :
molto simile alla gestione file perche  i driver di gestione sono gli stessi
essendo definte dal driver del sistema operativo adotta le stesse api adottate nel file system

write (fd[1], messaggio , lunghezza ) -> per scrivere il messsaggio nella pipe
read(fd[0], messaggio , lunghezza) -> per leggere dalla pipe
 close(fd[1]); -> chiudo il canale della pipe
 mkfifo("pippo", S_IRUSR|S_IWUSR); -> creo la named pipe con nome pippo e permessi di lettura e scrittura dagli utenti

58) int pipe (int fd[2]) -> invoca la creazione di una PIPE . vengono restituiti 2 puntatori 1 per la lettura della pipe e uno per la scrittura della pipe

59) int makefifo(char *name , int mode ) -> crea una pipe con nome
// piu generale in quanto puo essere presa da un altro processo prendone il descrittore di ritorno si apre una fifo tramite la chiamata OPEN()

///////////////////////////////////MESSAGGI ////////////////////////

NOTA (viene definita una struttura per il messaggio)

typedef struct{ // struttra del messaggio
        long mtype; // tipo di messaggio  1 regular 2 synch (REGULAR_MESSAGE, SYNCH_MESSAGE)
        char mtext[TAGLIA]; // talgia del messaggio
    } msg;


60) int msgget(key_t key , int flag ) -> crea la coda dei messaggi  mi torna il descrittore della coda se e stata creata altrimenti -1

            1) key_t key -> discrimina ke varie code di messaggi create
            2) flag -> IPC_CREAT se chiamo questa e la coda non esite allora ne creo una nuova
                       IPC_EXCL l operazione deve essere realmente eseguita se la coda esiste mi torna errore
                       permessi di accesso in codifica attuale

61) int msgctl(int ds_coda  , int cmd , struct msqid_ds *buf) -> invoco un comando su la coda dei messaggi
            1) ds_coda -> descrittore della coda
            2) cmd -> comadno da eseguire: IPC_RMDI -> invoca la rimozione della coda  //msgctl(des_coda, IPC_RMID, NULL); lo faccio all aifne come le pipe che cancello la coda
                                           IPC_STAT -> info su lo stato della coda
                                           IPC_SET  -> settaggio delle informazioni
            3) bud -> puntatore al buffer con eventuali parametri per il comando

62) int msgsnd(int ds_coda , const void *buff , size_t nByte , int flag) -> invoca la spedizione di un messaggio su una coda
            1) ds_coda -> descrittore della coda
            2) buff ->  puntatore al buffer  dove contengo il messagggio
            3) nByte -> taglia del messaggio (del buffer )
            4) flag -> opzione di spedizione  (IPC_NOWAIT -> indica che tale chiamata non e bloccante )

63) int msgrcv(int ds_coda , const void *buff , size_t nByte ,long type,  int flag) -> invoca la ricezione di un messaggio su una coda
            1) ds_coda -> descrittore della coda
            2) *buff ->  puntatore al buffer  dove conterro il messagggio scaricato
            3) nByte -> taglia del messaggio (del buffer )
            4) type -> tipo del messaggio che devo ricevere : 0  -> nessun tipo di messaggio specifico quindi la coda di messaggi mi lavora in FIFO
                                                              >0 -> mi restituisce il primo messaggio della coda del tipo specifico
                                                              <0 -> interessato a tutti i messaggi con valore minore in modulo
            5) flag -> opzione di spedizione  (IPC_NOWAIT) -> indica che tale chiamata non e bloccante

/////////////////////////////////////GESTIONE DELLA MEMORIA -> MAPPING  ////////////////////////////////////////


64) void *mmap(void *addr , size_t lenght , int prot , int flags , int fd , off_t offset) -> VALIDA (rende accessibili) regioni di pagine contigue

1) *addr ->  indirizzo INIZIALE  della pagina da mappare
2) lenght -> taglia della regione da mappare in byte

3) prot ->  tipo di protezione alla regione   :  1) PROT_EXEC -> protezione per l esecuzione
                                                 2) PROT_READ -> protezione per la lettura
                                                 3) PROT_WRITE -> protezione di scrittura
                                                 4) PROT_NONE -> protezione di non utilizzo

4) flag -> campo opzioni : 1) MAP_PRIVATE  -> mappaggio della regione privato (non condiviso )
                           2) MAP_SHARED  -> mappaggio della regione condiviso
                           3) MAP_ANONYMOUS -> mappaggio anonimo

5) fd ->descrittore verso un file da immettere nella regione mappata
6) offset -> posizione del file pointer da dove iniziare a mappare

la restituzione non e altro che l inizio della regione dell area mappata
NULL -> FALLISCE

65)int munmap(void *addr , size_t lenght) -> rimuove le pagine mappate nell address space e in memoria fisica
            1) addr -> pintatore all area di memoria da un-mappare
            2) lenght -> taglia della regione da un-mappare dall id base di address
-1 -> fallimento

66) int mprotect(void *addr , size_t lenght , int prot) -> cambia la protezione dell area di memoria impostata in fase di instanziazione

      1) addr -> indirizzo base
      2) lenght -> lunghezza area di memoria
      3) prot -> tipo di protezione (specificato nella API 64)

67) int brk(void *addr)-> ridefinisco il punto di rottura del bss passando il nuovo puntatore

68) void *sbrk(intptr_t incremento) -> passo direttamente l incremento senza preccuparmi di cercare l indirizzo

//////////////GESTIONE DELLA MEMORIA -> MEMORIA CONDIVISA //////////////////////////


69) int shmget(key_t key , int size , int flag) -> invoca la condivisione di un area di memoria (usata tra piu processi)

      1)key -> chiave per idernificarel amemoria condivisibile in maniera univoca nel sistema
      2)size -> taglia in byte della memroia codivisibile
      3)flag -> modalita di cerazione 1) IPC_CREAT  , 2) IPC_EXCL

SUCCESSO -> mi rida il descrittore dell area di memoria mappata e condivisa
FALLIMENTO -> -1


70) int shmctl(int ds_shm , int cmd , struct shmid_ds *buff) -> invoca l esecuzione di un comando sull area di memoria

      1) ds_shm -> descrittore dell area di memoria condivisa
      2) cmd  -> specifica del comando da passare  1) IPC_RMID -> invoca la rimozione della memoria
                                                   2) IPC_STAT -> info stato corrente della memoria
                                                   3) IPC_SET -> set di permessi d accesso  , ecc
      3) buff -> puntatore al buffer con eventuali parametri per il comando

71) void *shmat(int ds_shm, void *addr , int flag) -> invoca il collegamento di una memoria condivisa allo spazio di indirizzamento (address space ) del processo
      1) ds_shm -> descrittore della memoria condivisa da collegare
      2) *addr -> indirizzo preferenziale per il collegamento (NULL -> default )
      3) flag -> campo delle opzioni 1) SHM_R -> lettura
                                     2) SHM_W -> scrittura
                                     3) SHM_RW -> lettura e scrittura
SUCCESSO  -> indirizzo di accesso in memoria
FALLIMENTO -> -1

72) int shmdet(const void *addr) -> invoca lo scollegamento di una memoria condivisa dall address space del processo (NON LA RIMUOVE MA LA SOLO SCOLLEGA )
            1) *addr -> indirizzo di memoria da scollegare


////////////////////////SINCRONIZZAZIONE -> SPINLOCK ////////////////////

tipo di variabile adottata

spinlock_t lock




73) int pthread_spin_init(pthread_spinlock_t *lock  , int pshared) -> inizializza il blocco (lock )

       1) *lock -> puntatore alla vriabile lock
       2) pahsred -> usato per sincronizzazione attiva di  processi distinti   : 1) PTHREAD_PROCESS_SHARED
                                                                                 2) PTHREAD_PROCESS_PRIVATE

74) spin_lock(&slock) -> prende il lock
75) spin_unlock(&slok) -> rilascia il lock

/////////////////////SINCRONIZZAZIONE ->SEMAFORI /////////////////////////////////


76) int semget(key_t key , int size , int flag) -> crea un semaforo (simile alle shared memory e alle code )
      1) key -> chiave che identifica il  semaforo nel sistema
      2) size -> quanti distrubutorii di token ci devono essere
      3) flag -> modalita di creazione 1) IPC_CREAT -> se esiste il semaforo viene aperto
                                       2) IPC_EXCL -> creacione esclusiva del semaforo (se esiste torna errore)

SUCCESSO -> torna il descrittore del semaforo aperto
FALLIMENTO -> -1

77) int semctl(int ds_sem , int sem_num , int cmd , union semun arg)
                  1) ds_sem -> descrittore del semaforo +
                  2) sem_num -> indirce dell elemento del semaforo su cui si vuole operare
                  3) cmd -> specifica del comando : 1) IPC_RMDI -> rimuove il semaforo
                                                    2) IPC_STAT -> stato del semaforo(permessi )
                                                    3) IPC_SET  -> imposto permessi semaforo
                                                    4) GETALL   -> prende i valori di tutti i semafori
                                                    5) SETALL   -> setta il valore di tutti i  semafori (1 , ..., 10 passati in arg)
                                                    6) GETVAL   -> prende valore 1 elemento
                                                    7) SETVAL   -> imposta valore 1 elemento
                  4) arg -> puntatore al buffer con eventuali argomenti da passate ulteriormente

78) int semap(int ds_sem , struct sembuf *oper[] , int number  ) -> invoca l esecuzione di un comando su una coda di messaggi
            1) ds_sem -> descrittore del semaforo coinvolto
            2) oper[] -> indirizzo dell array cotenente la specifica delle operazioni da eseguire
            3) number -> numero di elementi dell array di semafori che vogliamo coinvolgere (specifica il numero di semafori da coinvolgere)

///////////////////////LISTA VARIANTI DEI SEMAFORI ////////////////////////////////

NOTA
 variabile usata  : sem_t sem_name

 in Unix si creano un arrray di semafori dove ogni elemento ha un numero di token

  token -> se 0 -> allora il thread o processo che vuole prendersi il token viene bloccato
           altrimenti -> piu processi o thread possono adoperare piu token per eseguire istruzioni in modo concorrente

l array di semafori puo essere un vantaggio enorme in quanto potrei assegnare ad ogni processo un semaforo e ad ogni thread
farò prendere i token in modo da avere anche lo spawn limitato dei thread ed evitare anche saturazione (IDEA)
RIMANE IL FATTO CHE ALCUNI SCONSIGLIANO DI ADOTTARE SEMAFORI ECC IN CODICI CONVENZIONALI PERCHE CREARE DEADLOCK O ALTRE STRONZATE E MOLTO SEMPLICE

79) sem_t *sem_open(const char *name , int flag) -> restituisce il puntatore al semaforo rappresentato dallo pseudo file
            1) name -> nome del semaforo(pseudo file)
            2) flag  -> eventuali opzioni

80) int sem_init(sem_t *sem , int pshared , unsigned int value ) -> inizializza tramite puntatore le info di come gestire il semaforo , tipo : condizioni , condivisioni

81) sem_waiy(&sem_name) -> prelevo un token dal semaforo

82) sem_post(&sem_name) -> rilascio il tocken

83) sem_getvalue(sem_t *sem , int *volp) -> prendo valore semaforo

84) sem_unlink (const char *name)  -> rimuove lo pseudo file con questo nome (rimuove l HARD LINK )


//////////////////////////// SINCRONIZZAZIONE -> MUTEX /////////////////////////////////

NOTA : tipo di variabile usata -> pthread_mutex_t mutex

MUTEX e un oggetto di mutua esclusione (SEMAFORO BINARIO )

85) int pthread_mutex_init(pthread_mutex_t *mutex , const pthread_mutexaddr_t *attribute) -> invoca l inizializzazione di un MUTEX (oggetto di mutua esclusione )

85.1) pthread_mutex_lock(pthread_mutex_t *mutex) -> invoca il blocco di un oggetto  mutex per farlo usare solo da un processo +

86) pthread_mutex_trylock(pthread_mutex_t *mutex) -> provo a vedere se un oggetto mutex e libero o no

87) pthred_mutex_unlock (pthread_mutex_t *mutex) -> sblocco l oggetto mutex e lo rendo disponibile



//////////////////// system call  EVENTI /////////////////////////////////


88) int kill (int pid , int segnale) -> richiede l invidio di uno specifico segnale ad un processo (pid)

89) int raise(int segnale) -> richiede l invio di un segnale al thread cje invoca questa API

90) unsigned alarm(unsigned time) -> invoca l invio del segnale SIGALARM  a se stessi


91) void (*signal(int sig , void (*ptr)(int)))(int ) -> specifica il comportamento da eseguire all arrivo di un segnale

92) int pause(void) -> blocco il thread in attesa dell arrivo di qualsiasi segnale



///////// set dei segnali ////////////////////////////////////////////

ke seguenti API si basano su la SIGNAL MASK -> maschera di bit dove con il flag 0 1 possiamo scegliere il segnale

93) int sigemptyset(sigset_t *set) -> flagga la signal mask tutta a 0

94) int sigfillset(sigset_t *set)  -> flagga ls signal mask tutta a 1

95) int sigaddset(sigset_t *set , int signal) -> flaggo a 1  il sengale corrispondente al numero che passo al secondo arogmento

96) int sigdelset(sigset_t *set , int signal) -> flaggo a 0  il segnale corrispondente al numero che passo al secondo argomento

97) int sigismember(sigset_t *set , int segnal) -> controllo il flag del segnale passato al secondo argomento

98) int segprocmask(int how, const sigset_t *set , sigset_t *oset ) -> imposta la gestione della signal mask
            1) how -> in che modo intervengo sulla SIGNAL MASCK : 1) SIG_BLOCK -> i segnali che indico in set saranno messi nella signal mask
                                                                  2) SIG_UNBLOCK -> i segnali indicati in set saranno rimossi dalla signal masck
                                                                  3) SIG_SETMASCK -> la nuova signal masck sara quella indicata nel set
            2) *set -> il set di segnali sui quali verranno fatte le modifiche nella SIGNAL MASCK
            3) *oset -> se non e NULL , nella relativa locazione verra scritto il valore della signal mask PRIMA di fare la modifica

99) int sigpending(sigset_t *set) -> restituisce l insieme dei segnali che sono pendenti

100)int sigaction(int sig , const struct sigaction *act , struct sigaction *act) ->permette di esaminare e/o modificare l’azione associata ad un segnale
      1) sig  -> segnale interessato della modifica
      2) *act (secondo argomento ) -> indica come modificale la gestione del segnale
      3) *act (terzo argomento ) -> vengono memorizzate le impostazionei precedenti con il gestore act signal

//////////////////SOCKET //////////////////////////

NOTA :
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

invocazionei del server  : SOCKET -> BIND -> LISTEN -> ACCEPT
invocazionei del client  : SOCKET -> BIND -> CONNECT

101)int socket(int domain , int type ,  int protocol) -> invoca la creazione della socket
            1) domain -> indica in che formato i datagrammi saranno  1) AF_INET -> indica che i datagrammi saranno in formato IPv4
                                                                     2) AF_INET6 -> indica che i datagrammi saranno in formato IPv6
                                                                     3) AF_UNIX -> per la l intercomunication interna al sistema unix

            2) type -> indico il tipo di connessione  1) SOCK_DGRAM -> usata per connessioni UDP
                                                      2) SOCK_STREAM -> usata per connessioni TCP

            3) protocol -> indica il protocollo  (e una specifica in piu) : 1) NULL -> autocompleta la chiamata vedendo il tyope
                                                                            2) IPPROTO_UDP -> specifica che la socket sarà UDP
                                                                            3) IPPROTO_TCP -> speficifa che la socket sara TCP

SUCCESSO  -> descrittore della socket
FALLIMENTO -> -1

102) int bind(int ds_sock , struct sockaddr *adr , int addrlen ) -> invoca l assegnazione di un indirizzo alla socket (da fare sempre dopo la creazione della socket e il settaggio della struttura dati sockaddr )

FALLIMENTO -1

103) int accept (int ds_sock , struct sockaddr *addr , int *addrlen) -> invoca l accettazione di una connessione su una socket  (questa genera un clone della socket che sara l intermediario con il client  )

SUCCESSO -> descrittore della nuova socket
FALLIMENTO -> -1

104) int listen(int ds_sock , int backlog) -> cambia lo stato della socket di connessione . inoltre con il secondo parametro indica il numero massimo di connessioni che puo accettare il server


105) int connect (int ds_sock   , struct sockaddr *addr , int addrlen) -> invoca la connessione alla socket del server

106) struct hostnet *gethostbyname (const char *name) ; -> utile per decdificare nomi host in indirizzi ip esempio se passo localhost torna  127.0.0.1 (NON VERIFICATO )

107) int socketpair(int domain , int type , int protocol , int sockvec[2]) -> invoca la creazione di 2 socket conensse viene usato solo comunicazione interna UNIX
                  1) domain -> AF_UNIX
                  2) type -> tipo di comunicazione
                  3) protocol -> protocollo di comunicazione socket
                  4) sockvec[2] -> descrittori per i 2 canali di comunicazione (passare un array di putatori a 2 interi )

NOTA SOCKET

close() -> per chiudere la socket

PREVIO USO DELLA CONNECT IN CONNESSIONI DI TIPO TCP POSSO ADOTTARE in quanto adotta delle socket CONNESSE (handshake fra server e client )

108) int send (int ds_sock , const void *buff, int size , int flag) -> invoca la spedidizone dei dati tramite socket
109) int recv(int ds_sock, const void *buff , int size , int flag)  -> invoca la ricezione dei dati tramite socket

PER CONNESIONI UDP ADOTTIAMO in quanto in UDP non viene fatto l uso di connessione ma viene mandato il pacchetto volante ad un mittente se lo riceve ok altrimenti sti cazzi

110) int sendto(int ds_sock , const void *buff , int size , int flag, struct sockaddr *addr , int addrlen) -> invoca la spedizione di dati tramite socket

111) int recvfrom(int ds_sock , const void *buff , int size , int flag , struct sockaddr * addr , int addrlen) -> invoca la ricezione di dati tramite socket

112) uint16_t htons (uint16_t htons16 bitvalue)-> invoca una conversione di  dati da HOST order a NETWORK order  (serve per rendere coerente i dati in rete tra LITTLE ENDIAN e BIG ENDIAN  )
113) uint32_t htons (uint32_t htons16 bitvalue)-> variante a 32 bit

114) uint16_t ntohs (uint16_t htons16 bitvalue)-> invoca una conversione di  dati da NETWORK order a HOST order  (serve per rendere coerente i dati in rete tra LITTLE ENDIAN e BIG ENDIAN  )
115) uint32_t ntohs (uint32_t htons16 bitvalue)-> variante a 32 bit

116) int setsocketopt(int sockfd , int level , int optname, void *optval, socklen_t *optlen ) -> impsta regole driver del socket
117) int getsocketopt(int sockfd , int level , int optname, void *optval, socklen_t *optlen ) -> prende le regole del driver del socket

API  a margine .

118) int inet_pton(inf of , const char *src , void *dst) -> converte indirizzi IPv4 o IPv6 dal formato testo in binario e lo mette nella struttura dati
119) int inet_aton(const char *cp , struct in_addr *inp) -> intent address e voglio che mi si impacchetti il codice in un altra area di memoria
120) in_addr_t inet_addr(const char *cp) -> puntando una stringa del tipo 192.168.0.125 mi restituira una stringa piu semplice
121) char *inet_ntoa(struct in_addr in) -> da codice numerico a stringa IP
122) in_addr_t inet_network(const char *cp) ->La funzione converte cp, una stringa nella notazione numeri e punti IPv4, in un numero nell'ordine dei byte host adatto

////////////////DI SEGUITO RIPORTERO LE VARIE API PIU DI NICCHIA TROVATE NEI SCRIPT //////////////////////////////

123) char *strtok(char *str, const char *delim); -> serve a etrarre delle sottostringhe (parole) da una concatenazione  di stringhe piu grande (frase)  nel campo delim passa il carattere che sara il delimitatore di ogni stringa

