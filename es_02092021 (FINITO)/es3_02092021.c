
/*
Esercizio 3
Si richiede di realizzare una funzione in C che permetta di modificare un vettore di interi andando a rimuovere tutti
gli elementi dispari.
La rimozione deve essere realizzata in place, ovverosia non è consentito effettuare copie del vettore. Il contatore di
elementi "validi" all'interno del vettore, preinizializzato all'interno del programma, deve essere aggiornato durante
l'eliminazione degli elementi.
*/

#include <stdio.h>
#include <string.h>
int vector[] = {21, 75, 73, 97, 98, 48, 2, 66, 52, 47, 44,6, 40, 41, 50, 24, 29, 1, 25, 46, 14, 42, 9 , 3, 71, 96, 61, 58, 10, 68, 20, 95, 32, 16};
size_t count = sizeof(vector)/ sizeof(int) ; // conta il numero di caratteri del vettore

void resize(int *v) // count non lo metto nella fuznione perche e una variabile globale quindi non serve passarla come argomento in quanto e come dio
{
    size_t size = count;
    for(size_t i= 0 ;i < size; i++){
        if(v[i]%2){
            memmove(&v[i], &v[i+1], (size -i)* sizeof(int)); // ricordarsi la memove
// dove passo gli indirizzi del valore puntato , il valore del intero successivo e l ultimo e il resize del vettore 33 - i * 4(dimensione di un intero )
            count--; // decremento il counter perche ovviamente il vettore e stato resize
            i--; // decremento i cosi non scorro in avanti e alla prossima iterazione controllo il nuovo valore
        }
        else
            continue; // altrimenti continua
    }
}
int main(void){

    resize(vector)
    for(size_t i= 0 ;i< count ; i++)
       printf("%d - " , vector[i] );
return 0 ;
}
