/*

il quick sort e molto utile ..

PIACCIONO i vettori piu random possibili  -- complessita O(n log n);
ODIA i vettori ordinati -- complessita O(N^2);

inoltre non e stabile
cioe quando fli vengono passati 2 valori con la stessa chiave memorizza prima l ultimo
elemento passato e pio messo l  elemento preesistente


COME FUNZONA : si adotta una regola euristica che per quasi tutti i casi ci migliora la complessita nel caso peggiore

1) Si sceglie l elemento centrale del vettore (pivot) e lo memorizza in una variabile

2) SX --> elementi >= pivot

3) DX --> elementi >= pivot

4) eseguiamo ricorsivamente la parte destra e sinistra fino a quando non e tutto ordinato


ogni volta ci sono 2 chiiamate ricorsive

la PRIMA - su la prima meta del vettore
la SECONDA - su la seconda meta del vettore .

e diciamo un estensione della ricerca bianria su vettori random  .

con tante iterazioni alla fine i 2 sottovettori richiamati diventeranno di un solo elemento e  li l algoritmo finisce dopo aver appurato la corretta posizione dei 2 elementi



 per raffronto grafico vedi

 https://www.codingcreativo.it/quick-sort-in-c/
*/


#include<stdio.h>
#define MAX 100

int insert_array(int a[]) {
  int n, i;
  printf("Quanti elementi?: ");
  scanf("%d", &n);

  for (i=0; i<n; i++) {
  	 printf("elemento %d: ", i);
  	    scanf("%d", &a[i]);
  }
  return(n);
}

void stampa_array(int a[], int n) {  // stampa array classico manco lo speigo
  int i;
  for (i=0; i<n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return;
}

void quicksort(int a[MAX],int primo,int ultimo){
   int i, j, pivot, temp;
/*pivot -- inizialmente il pivot è il primo elemento
primo e ultimo sono le due variabili che servono per scorrere l'array
*/
   if(primo<ultimo){ // se primo e minore di ultimo allora
      pivot=primo; // imposta il pivot
      i=primo;
      j=ultimo;

      while(i<j){ // finche i<j
         while(a[i]<=a[pivot]&&i<ultimo) // se l elemento della prima parte del vettore e minore del pivot e minore del ultimo
            i++; // incrementa i perche quella e la posizione corretta
         while(a[j]>a[pivot]) // se l elemtno della seconda meta e maggiore del pivot
            j--; // decrementa indice j perche quella e la posizione corretta
         if(i<j){ // se i < j
            temp=a[i]; // fai lo swap dei valori
            a[i]=a[j];
            a[j]=temp;
         }
      }

      temp=a[pivot]; // temp = elemento del vettore indicato dal pivot
      a[pivot]=a[j];   // il pivot diventa a[j];
      a[j]=temp; // a[j] = temp
      quicksort(a,primo,j-1); // chiamata ricordiva su la prima meta del vettore
      quicksort(a,j+1,ultimo); // chiamata ricorsiva su la seconda meta del vettore
   }
}

int main(){
   int n, a[MAX],i;
   n = insert_array(a);
   printf("Array iniziale: ");
   stampa_array(a,n);
   quicksort(a,0,n-1);
   printf("Array ordinato con quick-sort: ");
   stampa_array(a,n);
   return 0;
}
