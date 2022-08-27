#include <stdio.h>

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define N_PROCESS 7

void main (void){


    pid_t pid = 0 ;
    int process = N_PROCESS;
    int count = 0 ; // se i calocli
    int status;

    while (process-- > 0){ // creo n processi
        pid = fork();


        if(pid == 0){
            printf("sono il processo figlio ho il pid  e incremento il count ; %d %d \n",getpid() , count++);
            continue ;

        }else if(pid == -1){ // controllo della creazione del processo
            printf("impossibile generare il processo\n");
            exit(1);

        }else
            break;
    }
    printf("sono il processo padre e stampo il mio pid e il counter: %d -> %d \n", getpid() , count);
    wait(&status);
    printf("stampo la terminazione  del processo:  %d \n" , status>>8);
}




/*
e stato fatto un cascading di processi dove il processo padre usciva e stampava
il figlio incrementa ma al prossimo giro diventera padre e fara la stessa cosa.
operazione iterata per n-volte


sono il processo padre e stampo il mio pid e il counter: 40992 -> 0
sono il processo figlio e incremento il count ; 0

sono il processo padre e stampo il mio pid e il counter: 40993 -> 1
sono il processo figlio e incremento il count ; 1

sono il processo padre e stampo il mio pid e il counter: 40994 -> 2
sono il processo figlio e incremento il count ; 2

sono il processo padre e stampo il mio pid e il counter: 40995 -> 3
sono il processo figlio e incremento il count ; 3

sono il processo padre e stampo il mio pid e il counter: 40996 -> 4
sono il processo figlio e incremento il count ; 4

sono il processo padre e stampo il mio pid e il counter: 40997 -> 5
sono il processo figlio e incremento il count ; 5

sono il processo padre e stampo il mio pid e il counter: 40998 -> 6
sono il processo figlio e incremento il count ; 6

sono il processo padre e stampo il mio pid e il counter: 40999 -> 7
l ultimo non stqmpa in quando ha finito di generare i figli

tutti 0 quindi si presume che i processi siano andati tutti a buon fine
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0
stampo la terminazione  del processo:  0

 */
