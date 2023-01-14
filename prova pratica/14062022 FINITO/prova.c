/*Si sviluppi una applicazione che riceva tramite argv[] la seguente linea di comando

    nome_prog -f file1 [file2] ... [fileN] -s stringa1 [stringa2] ... [stringaN] 
    
indicante N nomi di file (con N > 0) ed N ulteriori stringhe (il numero dei nomi dei 
file specificati deve corrispondere al numero delle stringhe specificate).

L'applicazione dovra' generare N processi figli concorrenti, in cui l'i-esimo di questi 
processi effettuera' la gestione dell'i-esimo dei file identificati tramite argv[].
Tale file dovra' essere rigenerato allo startup dell'applicazione.
Il main thread del processo originale dovra' leggere indefinitamente stringhe da 
standard input e dovra' comparare ogni stringa letta con le N stringhe ricevute in 
input tramite argv[].
Nel caso in cui la stringa letta sia uguale alla i-esima delle N stringhe ricevuta 
in input, questa dovra' essere comunicata all'i-esimo processo figlio in modo che questo 
la possa inserire in una linea del file di cui sta effettuando la gestione. Invece, 
se il main thread legge una stringa non uguale ad alcuna delle N stringhe ricevute 
in input, questa stringa dovra essere comunicata a tutti gli N processi figli
attivi, che la dovranno scrivere sui relativi file in una nuova linea.

L'applicazione dovra' gestire il segnale SIGINT (o CTRL_C_EVENT nel caso
WinAPI) in modo tale che quando uno qualsiasi dei processi figli venga colpito 
dovra' riportare il contenuto del file da esso correntemente gestito in un file
con lo stesso nome ma con suffisso "_backup".  Invece il processo originale non dovra'
terminare o eseguire alcuna attivita' in caso di segnalazione.

In caso non vi sia immissione di dati sullo standard input e non vi siano segnalazioni, 
l'applicazione dovra' utilizzare non piu' del 5% della capacita' di lavoro della CPU.*/



#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 4096
#define MAX_PATH_SIZE 128
#define fflush(stdin) while(getchar()!='\n');


int s2, fd, fdo, id;
key_t sem_key_2 = 50;
key_t mem_key = 90;
char **backup_files;
char **files;

void printer(){
	
	char cmd[64];
	char buff[SIZE];

	
	//printf("pid: %d\n", getpid());
	
	int length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	
	
	if(read(fd, buff, length)<0){
		printf("Errore nella lettura dal file\n");
		exit(1);
	}
	
	if(write(fdo, buff, length)<0){
		printf("Errore nella scrittura sul file di backup\n");
		exit(1);
	}
	
	lseek(fdo, 0, SEEK_SET);
	
	puts("");
	sprintf(cmd, "cat %s", backup_files[id]);
	system(cmd);
	
	
	
}





int main(int argc, char *argv[]){
	
	
	signal(SIGINT, SIG_IGN);
	struct sembuf *oper = malloc(sizeof(*oper));
	pid_t pid;
	int ret, N = (argc-3)/2;
	int j, i, md;
	char *buff, *addr;
	int count;

	
	if(N<0){
		printf("Numero di argomenti al programma errato\n");
		exit(1);
	}
	
	if((s2 = semget(sem_key_2, N, IPC_CREAT|0777))<0){
		printf("%s\n", strerror(errno));
		printf("Errore nella creazione del semaforo per i processi figli\n");
		exit(1);
	}
	
	for(j=0; j<N; j++){
		
		if(semctl(s2, j, SETVAL, 0)<0){
			printf("Impossibile inizializzare il semaforo per i processi figli\n");
			exit(1);
		}
		
	}
	
	backup_files = malloc(N);
	files = malloc(N);
	for(j=0; j<N; j++){
		files[j] = malloc(MAX_PATH_SIZE);
		strcpy(files[j], argv[j+2]);
		backup_files[j] = malloc(MAX_PATH_SIZE);
		sprintf(backup_files[j], "%s_backup", argv[j+2]);
	}
	
	//signal(SIGINT, SIG_IGN);
	
	for(i=0; i<N; i++){
		
		pid = fork();
		
		if(pid<0){
			printf("Errore nella creazione del processo figlio\n");
			exit(1);
		}
		
		if(pid==0){
			
			signal(SIGINT, printer);
			//printf("%d: %s\n", i, backup_files[i]);
			id = i;
			
			if((fdo = open(backup_files[i], O_CREAT|O_WRONLY, 0777))<0){
				printf("Errore nell'apertura del file di backup\n");
				exit(1);
			}
			
			if((fd = open(files[i], O_CREAT|O_RDWR, 0777))<0){
				printf("Errore nell'apertura del file\n");
				exit(1);
			}
			
			if((md = shmget(mem_key+i, SIZE, IPC_CREAT|0777))<0){
				printf("Errore nella creazione della memoria condivisa\n");
				exit(1);
			}
			
			while(1){

redo:				
				oper[0].sem_num = i;
				oper[0].sem_op = -1;
				oper[0].sem_flg = 0;

				ret = semop(s2, oper, 1);
				
				if(ret==-1 && errno==EINTR) goto redo;
			
				
				char *temp = malloc(SIZE);
				
				if((addr = shmat(md, NULL, SHM_R))<0){
					printf("Errore nel collegamento del processo figlio alla memoria condivisa\n");
					exit(1);
				}
				
				memcpy(temp, addr, strlen(addr));
				temp[strlen(temp)]='\n';
				//printf("Figlio %d legge: %s\n", i, temp);
				
				if(write(fd, temp, strlen(temp))<0){
					printf("%s\n", strerror(errno));
					printf("Errore nella scrittura sul file\n");
					exit(1);
				}
				
				
			}
		}
	
	}
	
	//signal(SIGINT, SIG_IGN);
	
	while(1){
				
		printf("Inserire una stringa: ");
input:		
		while(scanf("%m[^\n]", &buff)<0){
			if(errno==EINTR) goto input;
			else{
				printf("Errore nell'inserimento dell'input\n");
				exit(1);
			}
		}
		
		
		count=0;
		
		fflush(stdin);
		
		for(j=0; j<N; j++){
			
			if(strcmp(buff, argv[N+3+j])==0){
				
				if((md = shmget(mem_key+j, SIZE, IPC_CREAT|0777))<0){
					printf("Errore nella creazione della memoria condivisa\n");
					exit(1);
				}
				
				if((addr = shmat(md, NULL, SHM_W))<0){
					printf("Errore nel collegamento del processo figlio alla memoria condivisa\n");
					exit(1);
				}
				count++;
								
				strcpy(addr, buff);
				
				oper[0].sem_num = j;
				oper[0].sem_op = 1;
				oper[0].sem_flg = 0;

				semop(s2, oper, 1);
											
			}
			
		}

		if(count==0){
			
			for(j=0; j<N; j++){
				
				if((md = shmget(mem_key+j, SIZE, IPC_CREAT|0777))<0){
					printf("Errore nella creazione della memoria condivisa\n");
					exit(1);
				}
				
				if((addr = shmat(md, NULL, SHM_W))<0){
					printf("Errore nel collegamento del processo figlio alla memoria condivisa\n");
					exit(1);
				}
				
								
				strcpy(addr, buff);
				
								
				oper[0].sem_num = j;
				oper[0].sem_op = 1;
				oper[0].sem_flg = 0;

				semop(s2, oper, 1);	
		
			}
			
			
		}
		
			
		
	}
	
}