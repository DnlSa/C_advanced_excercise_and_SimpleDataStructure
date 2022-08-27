/* HEAP SORT

esegue un ordinamento basato su una struttra chiamata HEAP ;
un heap e rappresentabile come un albero binario i cui nodi seguono una certa
PROPRIETA --> priorita.
e completo fino al penultimo livello



HEAP -> DECRESCENTE se ogni nodo padre contiene un valore maggiore del nodo figlio .
di conseguenza risulta che il padre abbia la chiave maggiore .

STRUTTURA USATISSIMA PER ORDINARE ARRAY;

in questo caso consideriamo come radice l elemento iniziale di indice 1 ;
i figli rispettivi avranno indice 2j(SX) , 2j+1(DX);

 L' algoritmo di seguito implementato crea inizialmente un heap decrescente .
 per ogni iterazione di copia la radic3*e in fondo alla lista dell array stesso ed esegue lo scambio di elementi .
 l algoritmo poi restituisce un heap di n-1 elementi spostando verso il basso la nuova radice e ricomindia con un altro scambio . esegue un ciclo che considera array di dimensioni decrescente

 COMPLESSITA COMPUTAZIONALE

 si dimostra che la complessita masima dell heap_sort e O(n log n);

 anche se l algoritmo di merge e quick sono piu veloci .

 PER ARRAY DI PICCOLE DIMENSIONI E PERSINO PIU VELOCE DELL INSERTION SORT
 nonostante abbia una complessita di O(N^2);
*/



#include <stdio.h>
#define MAX 20

int sinistro(int i)
{
 return 2*i+1;
}

int destro(int i)
{
 return 2*i+2;
}

int padre(int i)
{
 return (int)(i-1/2);
}

stampavettore(int *vettore,int n)
{
 int i;

 for(i=0 ; i<=n ; printf("%d ",vettore[i++]));
}

int riempivettore(int *vettore)
{
 int i;

 i=0;
 do {
 printf("inserire l'elemento %d dell'array('-1' per terminare): ",i+1);
 scanf("%d",vettore+i);
 } while (vettore[i++] != -1);
 return i-2;
}

void scambia(int *n1,int *n2)
{
 int temp;

 temp = *n1;
 *n1 = *n2;
 *n2 = temp;
}

void heapify(int *vettore, int i,int heapsize)
{
 int l,r,maggiore,violazione=1;

 while (violazione)
 {
 l = sinistro(i);
 r = destro(i);

 if ((l <= heapsize) && (vettore[l] > vettore[i]))
 {
 maggiore = l;
 }
 else
 {
 maggiore = i;
 }

 if ((r <= heapsize) && (vettore[r]>vettore[maggiore]))
 {
 maggiore = r;
 }
 if (maggiore != i)
 {
 scambia(&vettore[i],&vettore[maggiore]);
 i=maggiore;
 }
 else
 {
 violazione = 0;
 }
 }
}

void buildheap(int *vettore,int heapsize,int n)
{
 int i;

 for (i=(int)(n/2) ; i>=0 ; i--)
 {
 heapify(vettore,i,heapsize);
 }
}

void heapsort(int *vettore,int n)
{
 int i,heapsize;

 heapsize=n;
 buildheap(vettore,heapsize,n);
 for (i=n ; i>0 ; i--)
 {
 scambia(&vettore[0],&vettore[i]);
 heapsize--;
 heapify(vettore,0,heapsize);
 }
}

int main(void )
{
 int vettore[MAX];
 int n; /*numero di elementi*/

 n=riempivettore(vettore);

 heapsort(vettore,n);

 stampavettore(vettore,n);
}
