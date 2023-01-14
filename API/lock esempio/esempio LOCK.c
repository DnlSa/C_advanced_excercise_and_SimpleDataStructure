#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


#define  N 10

static const char  *filename = "danilo";

int main(void )
{
    pid_t pid;
    int len , des  , status, i, k;
    struct flock fl; // /struttura per fare il lock del file
    char text[10];// bufferino per accogliere il pid


// prima cosa che faccio e  aprire un file
    if( (des = open(filename, O_CREAT|O_TRUNC|O_WRONLY, 0660)) == -1){
        perror("errore nel apertura o creazione del file : ");
        exit(1);
    }
// creiamo i processi figli
    for(i=0 ; i<N ; i++){

        if( (pid = fork()) == -1){ // controllo dell errore
            perror("errore nel fork del processo");
            exit(-1);
        }


///////////////////// parte di codice dove viene applicato il lock////////////////////////////////////////////////////////////

        else if(pid == 0 ){ // sono il pro cesso figlio

            pid = getpid(); // prende il pid del processo
            len = sprintf(text, "%d", (int)pid); // funzione che stampa il numero di caratteri letti e mette dentro un bufferino

            // fcntl per poter lavorare in modo esclusivo sul file dobbiamo poter impostare il lock sul file stesso allora tramite fcntl impostiamo la strututr a
            fl.l_type = F_WRLCK;
			fl.l_whence = SEEK_SET;
			fl.l_start = 0;
			fl.l_len = 0;
			fl.l_pid = 0;

            if(fcntl(des , F_SETLKW , &fl) == -1){ // passo la variabile
                   printf("unable to lock the file %s \n ", filename);
                   exit(-1);
            }

            for(k = 0 ; k<i ; k++){
               if( write(des , text , len) == -1)
                   perror("write on file failed");
              }

               if( write(des , "\n\n", 2) == -1)  // scrivo anche 2 new line
                   perror("write on file failed");


               fl.l_type = F_UNLCK; // sblocco esclusivita su file

               if(fcntl(des , F_SETLKW , &fl) == -1){ // aggiorno la struttura del lock del file
                   printf("unable to lock the file %s \n ", filename);
                   exit(-1);
               }
                close(des);
                exit(0);
            }
        }


  //////////////////////////////////////////////////////////////////////////////////////////////







    for(i = 0 ; i<N ; i++){
        if(wait(&status) == -1){
            perror("errore in qualche processo figlio \n");
        }
    }
    close(des); // chiudo descrittore
    exit(0); // uscita
}
