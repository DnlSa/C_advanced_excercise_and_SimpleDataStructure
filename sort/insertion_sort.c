/*
L INSERTION SORT

l insertion e un algoritmo che se giochi a 3/7
 e lo stesso modo che si usa per ordinare le carte quando si pescano dopo che sono state servite

 opera facendo un tipo di ordinamento in loco . e utile se abbiamo degli array
 semi ordinati e motlo svantaggioso se abbiamo robba troppo random

 1) PASSO
 l algoritmo ha bisogno di 2 indici
 a) punta all elemento da ordinare
 b) quelloimmediatamente precedente

 2) PASSO
    confronta i 2 elementi puntati se il precedente e maggiore del successivo scambia

 3) itera finche non lo porta nella sua posizione originale

L array adotta una quantita minima di memeoria ma e molto lento se
l array fosse di grandi dimensioni .

PARTICOLARMENTE INDICATO SU PICCOLI ARRAY E SE HO CALCOLATORI
CON MEMORIA MOLTO LIMITATA
effettua in media confronti ed altrettanti spostamenti (mezzi scambi )
che possono diventare il doppio nel caso peggiore .
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define SCALE 100000000




int main(void)
{
    int i ,j , temp ;
    int n = 10;
    int arr[n]; // definisco un oggetto wrapper CHIAMATO s
    for(i =0 ; i<n ; i++){
        srand(tine(0));
        arr[i]= 1+rand()%100;
        // POPOLARE CON NUMERI RANDOM L ARRAY DI 10 ELEMENTI


    printf("L'array proma dell insertion sort  è : ");
    for(i =0 ; i<n ; i++)
        printf("%d ", arr[i]);
    printf("\n");



    for(i =1 ; i<n ; i++){ // conta da 1 a n
        temp = arr[i]; // appoggio alla variabile temporanea
        j= i-1; // imposto la j

    // sto while serve per far tornare indietro gli elementi della array
        while(j>=0 && arr[j]>temp){ // se j >= 0 e si vwerifica la condizione di scambio allora
            arr[j+1]=arr[j]; // scambia
            j--; // decrementa j cosi che l elemento appena scambiato venga confrontato con il precedente
        }
        arr[j+1]=temp;
    }



    printf("L'array dopo dell insertion sort  è : ");
for(i =0 ; i<n ; i++)
    printf("%d ", arr[i]);


printf("\n");
return 0;

}
