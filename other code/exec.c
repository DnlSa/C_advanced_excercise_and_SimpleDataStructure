#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
/*
  variamo della EXECVE() -- lanciare applicazioni
  esistono molte varianti la piu usata e EXECLP(.... )

  di fondo pero la system call e sempre 1 ed e equella elencata di su


  0) execve() -> e la sola e unica system call che il sistema esegue TUTTE LE ALTRE sono solo varianti (cambiano solamente cone vengono passati i parametri)

  1) PATHNAME  -> la prima basta specificare un path valido tipo usr/bin  e il nome dell applicativo perche lo prenda
     FILENAME  -> quesa invece prefice di specificare direttamente il nome dell applicativo che si trova dentro la stessa cartella di dove viene runnata questa API

 2) la shell -> ha dei comandi interni che anche se chiamo una execlp con ls  va bene
            -> ci sono altri comandi esterni che si trovano dentro /usr/bin
    la cosa la capiamo quando magari lancio il comando da shell
    python3 mod.py -> viene esattamente fatta una execl un usr/bin trova l applicativo che e python3 e passa in imput il file
     mi tornera il controllo della shell (CHE NON E ALTRO CHE IL SINGOLO PROCESSO ) SOLAMENTE quando finira di eseguire l applicativo .

     di solto in shell quando do pyton3 mod1.py  mi scrive RICEZIONE  e mi e impossibile impartire comandi sl terminale perche non ho piu il controllo
     cio vuol dire che la sheell essendo un unico processo e impegnata a fare qualcosa .
     per lavorare con una shell ne dovro aprire un altra e continuare ad interagire con il mio sistema

  3) la p finale  serve solo ad indicare che invece del filename passato come primo argomento tocca passare un percorso dove si trova l applicativo da eseguire



  p	--------------------------->Non serve specificare il path.

  v	--------------------------->Gli argomenti a riga di comando saranno passati alla funzione tramite un array di puntatori.

  e	--------------------------->L'insieme dei puntatori dell'environment è passato esplicitamente al processo figlio.

  l	--------------------------->Gli argomenti a riga di comando sono passati individualmente alla funzione.



*/

int execve(const char *pathname, char *const argv[], char *const envp[]);                              // VERA E UNICA SYSTEMCALL passa tuttto da questa

// di seguito metto ci sono solo api derivate

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 // l	--------------------------->      Gli argomenti a riga di comando sono passati individualmente alla funzione.


int execl(const char *pathname, const char *arg, ... /* (char  *) NULL */); // VARIANTE PATH
// 1) /bin/ls   2) argomenti addizionali come il nome dell aplicativo da cercare e magari anche qualche flag  "ls" , "-l".. 3) NULL oppure 0 ;

                        /*execl("/bin/ls","ls", "-l", (char *)0);*/





int execlp(const char *file, const char *arg, .../* (char  *) NULL */); // VARIANTE FILE  accetta  "ls" , "cd "

// 1) passo il nome dell applicativo  2) come argomenti passare di nuovo il nome dell apllicativo piu i vari flag  3) terminatore NULL oppure 0 ;

                            /*execlp("ls","ls", "-l", (char *)0);*/


int execle(const char *pathname, const char *arg, ... /*, (char *) NULL, char *const envp[] */); // VARIANTE PATH accetta "/bin/ls" MA CHE ACCETTA ANCHE VARIABILI D AMBIENTE


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// v	--------------------------->       Gli argomenti a riga di comando saranno passati alla funzione tramite un array di puntatori.


int execv(const char *pathname, char *const argv[]); // VARIANTE PATH   accetta "/bin/ls" , ecc

int execvp(const char *file, char *const argv[]); // VARIANTE FILE  accetta  "ls" , "cd "

int execvpe(const char *file, char *const argv[], char *const envp[]); // // VARIANTE PATH accetta "/bin/ls" MA CHE ACCETTA ANCHE VARIABILI D AMBIENTE


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void){

    if(execlp ("ls", "ls", 0 ) == -1){ // invoco applicativo con controllo annesso
        printf("errore nella chiamata \n");
        exit(-1);
    }
}
