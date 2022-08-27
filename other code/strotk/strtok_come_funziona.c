
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <sys/wait.h>
#include <stdlib.h>

#define COMMAND_LENGTH 1024 // lunghezza del comando




 // il conta argomenti e semplice
 // tockenizzo ecc
int main () {

    char *string;
    char copied_string[COMMAND_LENGTH + 1];
    char *temp ;
    int args = 0;

    printf("inserrisci uan stringa : ");
    scanf ("%m[^\n]",&string);


    strcpy(copied_string, string); // copio la stringa

    // la tockenizzo spezzando parola per parola (come fa la shell quando do lo spazio fra stringhe prende  codici separati )
    while (((temp = strtok(copied_string, " \n")) == NULL) || (strcmp(temp, "\0") == 0)) // la prima parola " \n" -> identifica lo spazio e il new line . "\0" indica il terminatore di stringa
        return(-1);

    printf("la strigna e : %s\n ", temp);
    args++;

    while (((temp = strtok(NULL, " \n")) != NULL)) { // tokenizza le successive stringhe fino alla fine
        printf("la strigna e : %s\n ", temp);
	    if (strcmp(temp, "\0") == 0 ) // quando attivo alla fine
           break;
	    args++;
    }
    printf("il numero di stringhe e : %d\n", args);
    return 0;
}
