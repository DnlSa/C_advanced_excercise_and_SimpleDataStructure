#include <stdio.h>


// DEFINISCO UNA STRUTTURA
struct node {

    int i ;
    node *next;

}

int empty(node *list) // QUESTO MI DICE SE LA LAISTA E VUOTA O NO
{
    return (list == NULL); // SE RITORNA 1 ALLORA LA LISTA E VUOTA PERCHE HEAD == NULL

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
        return (list);
}

// OPERATORE FRECCIA ACCEDE A UN CAMPO DI UNA STRUTTURA QUANDO DI QUELLA STRUTTUA NE CONOSCIAMO IL PUNTATORE
struct node *insert(struct node *pos , int k)
{
    struct node *new = (struct node * ) malloc (sizeof(node));
    new->i= k;
    new->next  = pos->next; // NUOVO NODO PUNTA AL PROSSIMO
    pos->next = new;  // IL PREV PUNTA AL NUOVO NODO
    // OPERATORE FRECCIA -> ACCEDO AL CAMPO NEXT DELLA STTUTTURA CHE E PUNTATA DA POS

// ANCHE QUESTA E UN OPERAZIOONE ORDINE DI 1 PERCHE NO CICLI NO ALTRA ROBA OPERAZIONALE .
// SOLO ASSEGNAMENTI
}

// scorre la lista fino alla coda  appena incontra che next == null  allora ritrona la fine della lista
// ha un costro lineare nella lista;

struct node *tail (struct node *list)
{
    asert(list!= NULL);

    while (list->next != NULL)
        list= list->next;

    return (list);
}

int main(int argc , char ** argv)
{
    head == NULL; // INIZIALIZZO TESTA A NULL PERCHE NON HO NODI
    struct node *list  = head  ; // INIZIALIZZO IL PRIMO NODO DELLA LISTA


}
