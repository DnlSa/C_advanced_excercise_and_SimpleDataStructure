#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node
{
    int data;
    struct tree_node *parent;
    struct tree_node *leftchild;
    struct tree_node *rightChild;
}node_t;

// con un nodo globale posso evitare di passare sempre la radice nelle funzioni in quanto presente da per tutto
node_t *root = NULL ; // VARIABILE GLOBALE RADICE E INIZIALIZZARLA A NULL
// inoltre ci garantisce di avere un solo abero nel mio software .
// inoltre eviterei di dichiarare 2 volte la stessa radice

// primo caso e stato descritot
// altrimenti devo salire fino a quando ho il sottoalbero da dove parto a sinistra
//




///////

// o ho un figlio destro ed e il minimo del figlio destro
// altrimenti sara quel nodo tale che avra il sottoalbero a  sinsitra
//  se sono sinistro al massimo e la radice
// se sono l ultimo figlio destro non ce l ho



node_t *min(node_t *T){ // se deve tornare un nodo , accertarsi che mi ritorna NULL
    if(T==NULL){
        printf("il nodo e nullo ");
        return NULL ;
    }


    while(T->leftChild!=NULL){
        T=T->leftChild; // // scorro nodo a sinistra

    }
    return T;
}



node_t *max(node_t *T)
{

        if(T==NULL){
        printf("il nodo e nullo ");
        return NULL ;
    }


    while(T->rightChild!=NULL){
        T=T->rightChild; // scorro nodo a destra

    }
    return T;
}



node_t *search( int x )
{
   node_t *T = root;
   if(T==NULL)
       return NULL;

   while (T->data != x){
       if(x<T->data)
            T = T->leftChild;
        else if (T == NULL)
            return NULL;
        else
            T= T->rightChild;
    }
}

// salgo semrpe a destra  fino a trovare un nodo che ha un figlio destro . vado nel nodo destro se il nodo destro ha un figlio sinistro vado a sinsitra
// salgo fino a trovare un parente con un figlio destro . scendo li  uina volta sceso chiamo il minimo
// voglio i piu piccoli dei figli di mio fratello


node_t *successor(node_t *T)
{
        if(T==NULL)
            return T;

        if(T->rightChild!= NULL) // se il figli destro non e nullo
                return min(T->rightChild); // passo la funzione minimo

        node_t *p = T->parent ; // uso il riferiemento a parent
        while(p!= NULL  && p->rughtChild==NULL){ // salgo e vedo se posso andare a destra .
            T = p;
            p= p->parent;
        }
return min(p->rightChild);
}


node_t *predecessor(node_t *T)
{


// vediamo dopo

}

void visitaPreOrd(node_t *T)
{
    if(T==NULL){
        printf("nodo NULL" );
        return;
    }
    visita(T);// stampa il nodo
    visitaPreOrd(T->leftChild); // si movuono a sinistra
    visitaPreOrd(T->rightChild); // si muovono a destra
}


void visitaInOrdine(node_t *T)
{
    if(T==NULL){
        printf("nodo NULL" );
        return;
    }
    visitaInOrdine(T->leftChild);
    visita(T);
    visitaInOrdine(T->rightChild);
}





