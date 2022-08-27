#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree {

    int key ;
  //  struct tree *parent; // nodo padre pensare all implementazione
    struct tree *leftChild; // nodo figlio sinistro
    struct tree *rightChild; // nodo figlio destro

}node_t;


void roots(node_t *T )
{
    printf("nodo radice e' : %d \n", T->key);
}



// funzione che va sempre a sinistra e trova il minimo
node_t *min(node_t *T )
{

    while(T->leftChild!=NULL)
         T= T->leftChild;

printf("il minimo e: %d\n", T->key);
return T;
}

node_t *MAX(node_t *T )
{

    while(T->rightChild!=NULL)
         T= T->rightChild;

printf("il massimo e: %d\n", T->key);
return T;
}

// data e la chiave che cerco nel mio albero
node_t *search(node_t *T, int data )
{
    if(T== NULL){
        printf("%d non è presente in lista \n" , data);
        return NULL;
    }else if(data < T->key)
        T= search(T->leftChild, data);
    else if (data > T->key)
        T= search(T->rightChild, data);
    else
        printf("la chaive cercata e presente nell albero alla poisizione : %p | %d\n ", T, T->key);

return T;
}


// l insert funziona in modo oche ad ogni chiamata ricorsiva
node_t *insert (node_t **T, int key)
{

    if((*T)!=NULL){
        if(key > (*T)->key){

            (*T)->rightChild = insert(&(*T)->rightChild, key);// VA A DESTRA


        }else if(key < (*T)->key){
            (*T)->leftChild = insert(&(*T)->leftChild, key);

        }else if(key == (*T)->key){
            printf("Nodo gia presente nel albero\n" );
        }

    }else{

        (*T) = malloc(sizeof(node_t)); // alloco lo spazio necessario per il nodo
        (*T)->key = key; // inserisco la chiave
        (*T)->leftChild= NULL;
        (*T)->rightChild = NULL ;


        }
return (*T);
}



// procedure di visita dei nodi
/*
 *
// le procedure di visita differiscono per qualche stronzata
// tipo nelle visite in profondita si usa o stesso codice solo si cambia
// la printf
PRE -- ordine   stampa     CHIAMATA      CHIAMATA
in -- ordine    CHIAMATA    stampa       CHIAMATA
POST -- ordine  CHIAMATA  CHIAMATA   stampa
*/
void in_order(node_t *T)
{

    if(T!=NULL){
        in_order(T->leftChild);
        printf("%d | " , T->key);
        in_order(T->rightChild);
    }
}

void post_order(node_t *T)
{
    if(T !=NULL)
    {
        post_order(T->leftChild);
        post_order(T->rightChild);
        printf("%d | " , T->key);
    }
}

void pre_order(node_t *T)
{
    if(T !=NULL)
    {
        printf("%d | " , T->key);
        pre_order(T->leftChild);
        pre_order(T->rightChild);
    }
}



int main(void)
{

    node_t *root = NULL ; // punto la radice a null
    int val[]= {1,2,100,42,3,55,4,5};
    int n = sizeof(val);
    n= n/4;
    int key;
    for(int i=0 ; i<n ; i++){
        key= val[i];
        insert(&root, key); // inserisco nodi
    }

    printf("Stampa dell albero: \n ");
    in_order(root);
    printf("\n");
    min(root);
    MAX(root);
    search(root,100);
    roots(root);
    printf("\n");
    return 0 ;
}
