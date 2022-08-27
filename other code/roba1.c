#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define N 6


/*
 ho dichiarato un array di thread (pthread_t tid[N]) al solo scopo di provare a spostare la pthread_join fuori dal ciclo e aspettare lo spawn dell ultimo trhead
 in questo modo mi da dei thread diversi .
 altrimenti l'identificatore è lo stesso , cio mi fa pensare al fatto di avere sempre lo stesso thread  e quindi instanzia la stessa TLD (thread local storage nello stack).


 il problema principale e che la scanf funziona solo con un thread (il primo ) .(di solito questo problema lo dava quando non flushavo l stdout , veniva trovato il file non vuoto che indica che c era qualcosa ancora da leggere)




 */




char *v[N]; // array di puntatori


void *thread_function( void *arg ){

  long i = (long) arg;
  pthread_t tid_2 = pthread_self() ; // prende id del thread
  printf("sono il thread numero : %ld %ld \n",tid_2 , i); // print di stampa del identificatore del thread e dell indice
  scanf("%m[^\n]", &v[i]);//per ogni elemento dell array di puntatori ne definisco una stringa
  pthread_exit(NULL); // terminazione del thread
}


int main(int argc , char **argv){

  pthread_t tid[N];
   int i;

  for (i=0 ; i<N ; i++ ){ // ciclo per lo spawn dei threads

    if(pthread_create(&tid[i] , NULL , thread_function , (void *)i)==-1){ // creazione del thread
      perror("failed to create thread");
      exit(1);
    }
  }
  pthread_join(tid[N-1] , NULL);  // wait del main thread fino all ultimo thread
    printf("\n");
  for (i=0 ; i<N ; i++ ) // ciclo di print delle stringhe immesse .
    printf(" il thread %d ha scritto %s\n" , i ,v[i]);

exit(0);
  }

