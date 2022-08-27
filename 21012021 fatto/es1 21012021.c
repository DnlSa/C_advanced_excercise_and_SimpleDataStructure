/* Dato un numero intero, scrivere un programma che restituisca una stringa in cui le cifre sono scritte
al contrario.
Ad esempio:
 input: 12345 (intero)
 output: "54321" (stringa)
Completare il codice indicato qui sotto. Attenzione: non modificate la firma della funzione di
conversione.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void capovolto(char *);


int main(int argc , char *argv[])
{
    int n= 12345; // l intero da convertire in stringa
    char *string2;

    int lenght =snprintf(NULL , 0, "%d",n); // crea la lunghezza per allocare la stringa
    char *string = malloc (lenght +1);  // alloca lo spazio +1 per il carattere di chiusura
    snprintf(string  ,lenght+1 , "%d",n); // converte da intero a stringa snprintf(stringa , lunghezza , tipo , numero)
    capovolto(string);
return 0 ;
}

void capovolto(char *string)
{
    int n= strlen(string); // numemero di elementi
    char string2[n];
    int count=0;

    for(int i=n-1  ;i>=0 ; i--){

        string2[count]=string[i];
        count++;
    }
printf("%s",string2);

printf("\n");
}
