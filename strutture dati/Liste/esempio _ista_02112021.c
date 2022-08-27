#include <stdio.h>

/*
 definisco una struttura il cuoi pimo nodo contiene 2 puntatori 1 alla testa e uno alla coda

 */

// DEFINISCO UNA STRUTTURA
struct node {

    int i ;
    stuct node *next;

};

// definiamo una seconda struttura che punta alla prima e definisco questi 2 puntatori testa e coda
struct list {
   struct node *head;
    struct node *tail;

};


// mnodifico la lista empty con la struc list
int empty(struct list *mylist)
{   // con operatore freccia vedo head dove punta
    return (mylist->head == NULL);

}

// MI DICE SE UNA LISTA E FINITA
int finished(struct node **list ) // POST E UN NODO SCORRITORE DELA LISTA
{
    struct node *pos= *list ; // stesso indirizzo della testa

    assert(pos != NULL); // assert comando di libreria nel quale controlla se pos  punta a null

    if(pos==NULL) // IMPOSTO STA CONDIZIONE SUL PUNTATORE POS CHE STA SCORRENDO LA LISTA SE E NULL
        return (true) ;

    return(pos->next ==NULL); // E FINITA SE VALE CHE L ULTIMO ELEMENTO CHE SCORRO E NULL


// pos head() -> torna un puntatore ;
struct node *head (struct node *list)
{
        return (mylist->head);
}

/*
int *head(struct list *mylist)
{
    return(mylist->head);
}
*/



// OPERATORE FRECCIA ACCEDE A UN CAMPO DI UNA STRUTTURA QUANDO DI QUELLA STRUTTUA NE CONOSCIAMO IL PUNTATORi
/* sta roba non funziona ... ci tocca implementare quando la lista e vuota*/
struct node *node_insert(struct  list *mylist , struct node *pos , int k)
{
    struct node *new = (struct node * ) malloc (sizeof(node));
    new->i= k;
    new->next=NULL;
    // primo caso da affrontare e la lista vuota
    // modifichiamo empty
    if(empty(mylist)){
        mylist->head=new;
        mylist->tail=new;

    }else{
        new->next = pos->next;
        pos->next = new;
        if(mylist ->tail == pos)
            mylist ->tail = new;
    }// corretto ammesso che pos indichi correttamente . non contempla pos == NULL




    new->next  = pos->next; // NUOVO NODO PUNTA AL PROSSIMO
    pos->next = new;  // IL PREV PUNTA AL NUOVO NODO

}



// scorre la lista fino alla coda  appena incontra che next == null  allora ritrona la fine della lista
// ha un costro lineare nella lista;
struct node *tail (struct list *mylist)
{
    node *current ;
    asert(list!= NULL);

    if(mylist->head == NULL){
        return (NULL);
    }

    while (list->next != NULL)
        list= list->next;

    return (list);
}

int main(int argc , char ** argv)
{
    struct list *mylist  = NULL  ; // INIZIALIZZO IL PRIMO NODO DELLA LISTA


}
