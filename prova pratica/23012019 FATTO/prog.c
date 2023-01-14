/******************************************************************
Welcome to the Operating System examination

You are editing the '/home/esame/prog.c' file. You cannot remove 
this file, just edit it so as to produce your own program according to
the specification listed below.

In the '/home/esame/'directory you can find a Makefile that you can 
use to compile this prpogram to generate an executable named 'prog' 
in the same directory. Typing 'make posix' you will compile for 
Posix, while typing 'make winapi' you will compile for WinAPI just 
depending on the specific technology you selected to implement the
given specification. Most of the required header files (for either 
Posix or WinAPI compilation) are already included in the head of the
prog.c file you are editing. 

At the end of the examination, the last saved snapshot of this file
will be automatically stored by the system and will be then considered
for the evaluation of your exam. Moifications made to prog.c which are
not saved by you via the editor will not appear in the stored version
of the prog.c file. 
In other words, unsaved changes will not be tracked, so please save 
this file when you think you have finished software development.
You can also modify the Makefile if requesed, since this file will also
be automatically stored together with your program and will be part
of the final data to be evaluated for your exam.

PLEASE BE CAREFUL THAT THE LAST SAVED VERSION OF THE prog.c FILE (and of
the Makfile) WILL BE AUTOMATICALLY STORED WHEN YOU CLOSE YOUR EXAMINATION 
VIA THE CLOSURE CODE YOU RECEIVED, OR WHEN THE TIME YOU HAVE BEEN GRANTED
TO DEVELOP YOUR PROGRAM EXPIRES. 


SPECIFICATION TO BE IMPLEMENTED:
Implementare una programma che riceva in input, tramite argv[], il nome
di un file F ed N stringhe S_1 .. S_N (con N maggiore o uguale
ad 1.


Per ogni stringa S_i dovra' essere attivato un nuovo thread T_i, che fungera'
da gestore della stringa S_i.


Il main thread dovra' leggere indefinitamente stringhe dallo standard-input.


Ogni nuova stringa letta dovra' essere comunicata a tutti i thread T_1 .. T_N
tramite un buffer condiviso, e ciascun thread T_i dovra' verificare se tale
stringa sia uguale alla stringa S_i da lui gestita.


In caso positivo, ogni
carattere della stringa immessa dovra' essere sostituito dal carattere '*'.
Dopo che i thread T_1 .. T_N hanno analizzato la stringa, ed eventualmente questa
sia stata modificata, il main thread dovra' scrivere tale stringa (modificata o non)
su una nuova linea del file F. 


In altre parole, la sequenza di stringhe provenienti dallo standard-input dovra' 
essere riportata su file F in una forma 'epurata'  delle stringhe S1 .. SN, 
che verranno sostituite da strighe  della stessa lunghezza costituite esclusivamente
da sequenze del carattere '*'.
Inoltre, qualora gia' esistente, il file F dovra' essere troncato (o rigenerato) 
all'atto del lancio dell'applicazione.



L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando il processo venga colpito esso dovra' 
riversare su standard-output il contenuto corrente del file F.

Qualora non vi sia immissione di input, l'applicazione dovra' utilizzare 
non piu' del 5% della capacita' di lavoro della CPU.

*****************************************************************/

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 4096
#define N 50


struct arguments{
	int  id;
	char * string;
};

char *buff;
int n_threads;
sem_t sem[N]; // do un numero fittizzio al thread
sem_t sem_main;
struct arguments argum[N]; // numero fittizio di strutture
int indice;
int modify;
char *filename;

void sirgnal_handler(int signo){
	char comando[4096];
	printf("\nil file ha le seguenti stringhe: \n");
	sprintf(comando ,"cat %s", filename );
	system(comando);
	printf("\nstringa: %s\n", comando);

	exit(EXIT_SUCCESS);
}

void *thread_function(void *arg){
	 int i, j;
	 char *string = (char *)arg; // anche questa ha

 redo:
	printf("sono il thread_CHILD aspetto  :  %d\n", indice);
	 sem_wait(&sem[indice]);
	printf("sono il thread_CHILD sbloccato:  %d\n", indice);
	 int len_1 = strlen(string);
	 int len_2 = strlen(buff);
	 char *str2 = buff; // buff non e modificato perche finge sempre da indirizzo base del buffer
	 char *str1 = string;
	 if(len_1 == len_2){ // se le stringhe hanno la stessa lunghezza allora faccio l epurazione e incrmento il semaforo del main thread che scrive e poi riprende la nuova stringa
		printf("ho trovato una stringa \n");
		 for (i=0 ; i<len_1 ; i++){
			for(j=0 ; j<len_2; j++){
				if((*str1)==(*str2)){
					(*str2) = '*';
					str2++;
				}else
					str2++;
			}
		str1++;
		str2=buff; // ad ogni ciclo riporto a 0 l indirizzo di str2 cosi posso riscorrerla di nuovo
		}
		str1 = string; // inutile
		modify =1;
 		printf("la stringa modificata e : %s\n", buff); // posso stampare tranquillamente

	 }

	if(indice == n_threads-1){ // indice va da 0 a 3
		indice =0;
		sem_post(&sem_main); // all ultimo thread di controllo passo il gettone al thread main che scrivera le modifiche
	}else{
		indice++; // do il token al prossimo semaforo
		sem_post(&sem[indice]);
	}
	goto redo; // ciclo

}


int main(int argc, char **argv){

	if(argc < 2){ // passo arrgomenti
		printf("usage : ./prog <filnemame> <string> <string> ....\n");
		fflush(stdout);
		exit(0);
	}
	signal(SIGINT , sirgnal_handler);

	filename = argv[1]; // memorizzo il filename
	n_threads = argc-2; // tolgo argv[0] e argv[1] che e il filename
	long i;
	int j, ret;
	printf("%d", n_threads);
	 buff = (char *)mmap(NULL , SIZE, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, 0,0 ); // area di memoria condivisa

	 sem_init(&sem_main , 0 , 1); // il semaforo main e attivo
	 for(i=0 ; i<n_threads ; i++){
		sem_init(&sem[i], 0 , 0 ); // tutti i semafori dei thread sono bloccati
	}

	pthread_t tid[n_threads]; // variabili di tipo threads
	j=0;
	for(i=2 ;i<argc ; i++){
		argum[j].string = argv[i]; // passo la stringa
		argum[j].id = j; // passo l indice del thread
		j++;
	}
	j=2;
	for(i=0 ; i<n_threads; i++){
		if((ret=pthread_create(&tid[i], NULL , thread_function ,(void*)argv[j])!=0)){
			perror("errore to create thread");
			exit(ret);
		}
		j++;
	}

	int fd;
	int count;
	int byte_w ;
	if((fd = open(filename , O_RDWR|O_CREAT|O_TRUNC , 06660))==-1){
		perror("error to open file");
		exit(-1);
	}

modify=0;
indice = 0; // reset dei thread di controllo
while (1){


   printf("sono il thread_main aspetto\n");
   sem_wait(&sem_main); // prendo il token

   if(modify){ //  se viene rilevata la modifica allora scriviamo la modifica altrimenti no
		if((byte_w = write(fd, buff , strlen(buff)))==-1){
			perror("write error");
			exit(-1);
		}
		modify=0;
	//printf("ho scritto sul file %d , %s\n\n", byte_w , buff);
   }
	sleep(0.5);
    printf("sono il thread_main inserisci una stringa : ");
    scanf("%[^\n]",buff); // lettura da stdinput e messo nella memoria condivisa
	getc(stdin);

	sem_post(&sem[indice]); // passo il primo indice

}
}
