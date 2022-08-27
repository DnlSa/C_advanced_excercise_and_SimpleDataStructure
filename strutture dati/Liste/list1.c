#include <stdio.h>
#include <stdlib.h> // mi serve per la malloc

struct node{

  int data; // campo numerico
  struct node *next; // campo puntatore a next
};




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

    printf("%d " , supervisor->data);
    while (supervisor->next != NULL)
        {
            supervisor=supervisor->next;
            printf("%d  " , supervisor->data);
        }
}

int main(int argc , char **argv)
{
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
    insert_sorted(&list_head , 25);
    insert_sorted(&list_head , 2);
   insert_sorted(&list_head , 24);
    control(&list_head);
    printf("\n");
    print_list(&list_head);
    printf("\n");
return 0;
}
