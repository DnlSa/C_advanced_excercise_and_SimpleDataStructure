#include <stdio.h>
#include <stdlib.h> // mi serve per la malloc

struct node{

  int data; // campo numerico
  struct node *next; // campo puntatore a next
};


int max(struct node *head , int key)
{
    if(head == NULL)
        return key;
    else{
        if(head->data > key)
            key= head->data;
        max(head->next , key);
    }
}

// prima funzione che voglio implementare e boolean empty
// gli passo il puntatore alla testa
int boolean_empty(struct node *list_head)
{
    return(list_head == NULL);
}


// mi serve per capire se una lista e finita
// posso dire che tale lista e finita se esiste un nodo che puntera a null
int boolean_finished(struct node *list_head)
{
    int k=0;
    struct node *temp = list_head; // imposto lo stesso indirizzo di della testa
    // un vettore e un puntatore se ne specifico solo il nome ne sto passando l indirizzo

    while(temp!=NULL){
        temp= temp->next;
        k++; // conta i nodi che passo
    }return k;
}

// mi deve tornare l indirizzo della testa
struct node *Pos_head(struct node *list_head)
{
    return(list_head);
}

struct node *Pos_tail(struct node *list_head)
{
    struct node *temp= list_head;
    while(temp!=NULL)
       temp= temp->next;

    return(temp);

}
void control(struct node **list_head)
{      int k=0;
    // passo i puntatori perche nella funzione passo il doppio puntatore quindi devo dereferenziare
    // qui non mi interessa passare il puntatore esatto anche perche e una funzione il cui unico scopo e
    // stampare delle informazioni senza che modifichi nulla
    if(boolean_empty(*list_head))
        printf("la lista e vuota\n");
    else
        printf("la lista non e vuota\n");

    k=boolean_finished(*list_head);
    printf("la lista ha %d nodi\n",k );
    printf("indirizzo della testa : %p\n",Pos_head(*list_head));
    printf("indirizzo della coda : %p\n", Pos_tail(*list_head));

}

// voglio creare un inserimento in testa
void insert_head(struct node **list_head, int i)
{

    struct node *node_head;
    node_head= malloc(sizeof(struct node)) ; // preservo spazio per allocare sia campo dati che puntatore al prossimo elemento

    if(node_head ==NULL)
        exit(EXIT_FAILURE);

    node_head->data = i; // salvo valore nel campo data
    node_head->next= *list_head; //
    *list_head = node_head;
}


void insert_tail(struct node **list_head , int i )
{
    struct node *node_tail;
    struct node *current; // creare un puntatore che navighi la lista e tenga conto del precedessore

    node_tail= malloc(sizeof(struct node));

     node_tail->data = i; // intanto popolo il nuovo nodo

     current = *list_head;

     if(*list_head== NULL){ // condizione primaria caso in cui on ci sono nodi
        node_tail->next = *list_head;
        *list_head = node_tail;
        return;
     }
    while(current->next != NULL)
        current = current->next;

    current->next = node_tail; // punta al nuovo nodo
    node_tail->next = NULL ; // lo faccio puntare a null per definizione
}

// algoritmo di inserimento di un nuodo qualsiasi
void insert_sorted(struct node **list_head, int i)
{
    struct node *current;
    current = *list_head;

    if(current == NULL || current->data > i ){
        insert_head(&(*list_head),i);
        return;
        }
 // deve scorrere la lista fino a
    while(current->next != NULL && current->next->data < i ){
        current = current->next; // avanzo
        }

    if(current == NULL){
            insert_tail(&(*list_head),i);
            return;
            }

    struct node *new_node;
    new_node= malloc(sizeof(struct node));
    new_node->data = i;
    if(new_node==NULL)
        exit(EXIT_FAILURE); // VEDERE BENE STO COMANDO

    new_node->next=current->next;
    current->next= new_node;
}

void print_list(struct node **list_head)
{
    struct node *supervisor;
    supervisor = *list_head;

    while (supervisor->next != NULL){
            printf("%d id - %p \n" , supervisor->data,supervisor->next);
            supervisor=supervisor->next;
        }
}

/* // stampa lista nodo successivo e nodo corrente
void print_list(struct node **list_head)
{
    struct node *supervisor;
    supervisor = *list_head;

    printf("stampo il nodo corrente su cui mi trovo ");
    printf("supervisor->data e supervisor->next\n");

    while (supervisor->next != NULL)
        {

            printf("%d id - %p \n" , supervisor->data,supervisor->next);
            supervisor=supervisor->next;

        }

    printf("\n \n");
    printf("stampo il nodo successivo rispetto al mio nodo  ");
    printf("supervisor->next->data e supervisor->next->next\n");
    supervisor = *list_head;
    al nodo successivo penso che io non possa fare operazioni in quanto lo posso solo interrogare su valore e indirizzo
    passare nella free current->next ecc e na stronzata colossale
     while (supervisor->next != NULL)
        {
            printf("%d id - %p \n" , supervisor->next->data,supervisor->next->next);
            supervisor=supervisor->next;
        }
}
*/




void delete(struct node **list_head, int i) // passo sia la testa come punto di inizio da dove partire e i deve essere l elemento da cancellare
{
    struct node *current ;
    current= *list_head; // punto il nodo corrente nella lista
    struct node *prev;

    prev=current; // metto il prev dietro
    current= current->next; // lo posiziono una posizione in avanti

    while (current->data != i){ // current trova il nodo successivo che e il target che voglio cancellare
        current = current->next;

        if(current->next == NULL){
            printf("Non esiste tale nodo nella lista" );
            return;
        }
    }
    prev->next=current->next; // e salto current

    free(current); //prima della free devo poter ricollegare i nodi
}

void libera_completamente_la_memoria(struct node **list_head)
{
    while((*list_head) !=  NULL)
    {
        struct node *temp = *list_head; // questo rimane indietro
        *list_head = (*list_head)->next; // questo va avanti di una posizione
        free(temp);
    }
}
int main(int argc , char **argv)
{   int key=0,massimo;
    struct node *list_head= NULL;
    control(&list_head);
    printf("\n");
    insert_head(&list_head, 23); // funziona

/*
 *   perche devo passare l indirizzo di head necessariamente !!
 * altrimetni passo un acopia del puntatore e modifico la copia che e abbastanza inutile
 */

    insert_sorted(&list_head, 4);
    insert_tail(&list_head, 3);
     insert_tail(&list_head, 300);
    insert_sorted(&list_head , 25);
    insert_sorted(&list_head , 2);
    insert_sorted(&list_head , 24);
    insert_sorted(&list_head , 1);
    massimo = max(list_head , key);
    printf("IL MASSIMO DELLA LISTA E : %d ", massimo);

    printf("\n");
    printf("i nodi prima della cancellazione sono :\n ");
    print_list(&list_head);

    delete(&list_head , 4);
    printf("\n");
    printf("i nodi dopo la cencellazione sono : \n ");
    print_list(&list_head);
    control(&list_head);
    printf("\n");
    printf("\n");

    libera_completamente_la_memoria(&list_head);
    control(&list_head);
return 0;
}
