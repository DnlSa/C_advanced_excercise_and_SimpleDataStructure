 
/*
 Esercizio 1
Si chiede di realizzare una struttura dati in C, secondo lo schema fornito nel file esercizio1.c, che implementi una
stringa dinamica.
Una stringa dinamica è una stringa che può essere utilizzata in una funzione di append(), tale per cui diventa
possibile concatenare più stringhe tra di loro durante l'esecuzione del programma.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef struct node{
    int buff ; // conta il numero di caratteri che la struttura puo avere
    int used ; // conta il numero di caratteri che sono stati usati
    unsigned char c[]; // contiene la stringa ;
}node_t ;




void init_string(node_t **string , int buff)
{
    (*string) = malloc(sizeof(node_t)+buff+1);
    (*string)->buff = buff;
    (*string)->used = 0;
    (*string)->c[0] = '\0';
}

void append(node_t **string , char *stringa )
{
    int len = (strlen(stringa)+1); // definisco la grandezza della stringa
    int i , j=0 ;
    (*string)->buff -= len-1; // sotraggo al buffer la parola
    if((*string)->buff <0){
        printf("spazio di memoria insufficente per allocare la parola desiderata: %s\n", stringa);
        exit(EXIT_FAILURE);
    }

    (*string)->used += len; // aumento il numero di caratteri usati
    while( (*string)->c[j] != '\0') // arriva alla fine della stringa giua allocata
        j++;

    for(i = 0 ; i<len+2 ; i++) // appende la stringa alla fine della precedente
        (*string)->c[i+j] = stringa[i]; // inserisco la stringa
}

int main(void )
{
        node_t *string;
        init_string(&string , 12);
        printf("hai allocato una strigna dinamica di : %d byte\n", string->buff); // funge qua
        append(&string , "ciao");
        printf("rimangono  : %d byte da allocare \n", string->buff); // funge qua
         append(&string , "ci");
        printf("rimangono  : %d byte da allocare \n", string->buff); // funge qua
        append(&string, "come");
        printf("rimangono  : %d byte da allocare \n", string->buff); // funge qua
         append(&string, "stai");
        printf("%s\n",string->c);
return 0;
}
