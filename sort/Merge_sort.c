/*

Merge Sort --> E stabile


e un algoritmo che sfrutta la tecnica
DIVIDE ET IMPERA ->

1) ovvero suddivido il mio problema in sotto-problemi
della stessa natura con una dimensione sempre piu piccola  , e li vado
a risolvere ricorsivamente . (PARTE DI SORT)

2) COMBINA la soluzione parziale di ogni problema . (PARTE DI MERGE).


QUINDI divido il vettore ricorsivamente in 2 parti da ordinare separatamente
dopo aver risolto . FONDO le due parti per ottenere un array ordinato globalmente

una mezza pecca e che deve usare un vettore di appoggio per poter lavorare


COMPLESSITA COMPUTAZIONALE

e sempre stabile a O(n log n ); -- su la parte del sort

la complessita del merge e O(N)

*/

#include <stdlib.h>
#include <stdio.h>

#define max 100

int insert_array(int V[]) {
  int n, i;
  printf("Quanti elementi?: ");
  scanf("%d", &n);

  for (i=0; i<n; i++) {
  	 printf("elemento %d: ", i);
  	    scanf("%d", &V[i]);
  }
  return(n);
}

void stampa_array(int V[], int n) {
  int i;
  for (i=0; i<n; i++) {
    printf("%d ", V[i]);
  }
  printf("\n");
  return;
}

void merge(int a[], int p, int q, int r) {
  int i, j, k=0, b[max];
  i = p;
  j = q+1;

  while (i<=q && j<=r) {
    if (a[i]<a[j]) {
      b[k] = a[i];
      i++;
    } else {
      b[k] = a[j];
      j++;
    }
    k++;
  }
  while (i <= q) {
    b[k] = a[i];
    i++;
    k++;
  }
  while (j <= r) {
    b[k] = a[j];
    j++;
    k++;
  }
  for (k=p; k<=r; k++)
    a[k] = b[k-p];
  return;
}

void mergeSort(int a[], int p, int r) {
  int q;
  if (p < r) {
    q = (p+r)/2;
    mergeSort(a, p, q);
    mergeSort(a, q+1, r);
    merge(a, p, q, r);
  }
  return;
}

int main(void) {
  int n, V[max];
  n = insert_array(V);
  mergeSort(V, 0, n-1);
  stampa_array(V, n);
  return(0);
}
