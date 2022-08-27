
// creare un lista doppiamente collegata

/* considererazione importante

    quando creo un lista e ne inserisco i 2 campo next e prev quelli sono puntatori che vanno dentro il nodo quindi usare
    current->prev = new_node->next ; E ASSOLUTAMENTE SBAGLIATISSIMO

    va puntato proprio il nodo MI RACCOOMANDO
    current->prev = new_node ; // COSI
*/

#include <stdio.h>
#include <stdlib.h> // mi serve per la malloc

struct node{

  int data; // campo numerico
  struct node *prev; // campo che punta al precedente
  struct node *next; // campo puntatore al successivo
};


// 1 return succes - 0 return fail
int insert_head(struct node **head, int i)
{
    struct node *new_node;
    new_node = malloc(sizeof(struct node));
    struct node *current;
        current = *head;

    if(new_node == NULL)
        return 0 ;

    new_node->prev=NULL;
    new_node->data = i;
    new_node->next = *head;//ok
    *head = new_node; // ok testa ripuntata a new_node

    if(new_node->next!=NULL){
        current->prev = new_node;
    }else
        ;

return 1;
}


int insert_tail(struct node **head , int i )
{
    struct node *new_node ;
    struct node *current ;

    new_node = malloc(sizeof(struct node));
     if(new_node == NULL)
        return 0 ;


    new_node->data = i;
    new_node->next = NULL; // PUNTO DIRETTO A NULL
    current= *head;



    // se metto il classico current->next arrivo all ultimo nodo
    while(current->next != NULL){ // punto 2 nodi avanti perhce a me interessa il campo next dell ultimo nodo e devo uscire prima che punto a NULL
        current = current->next;
    }

    current->next = new_node; // il capo next del current lo punto al prev
    new_node->prev = current; // punto il campo prev al current
    return 1;
}

struct node *print_list_front(struct node **head)
{
    struct node *supervisor;
    supervisor = *head;
    printf("node value -> %d   id_node_front -> %p    id_node_back -> %p \n", supervisor->data , supervisor->next , supervisor->prev);
    while(supervisor->next != NULL){

        supervisor = supervisor->next ;
        printf("node value -> %d   id_node_front -> %p    id_node_back -> %p \n", supervisor->data , supervisor->next , supervisor->prev);
    }
return supervisor;
}
void print_list_back(struct node **sup)
{
    struct node *supervisor;
    supervisor= *sup;
    printf("node value -> %d   id_node_front -> %p    id_node_back -> %p \n", supervisor->data , supervisor->next , supervisor->prev);

    while(supervisor->prev != NULL){
        supervisor = supervisor->prev ;
        printf("node value -> %d   id_node_front -> %p    id_node_back -> %p \n", supervisor->data , supervisor->next , supervisor->prev);
    }
}



int main(void)
{

    struct node *head = NULL ; // Punto la testa a null



    insert_head(&head, 6 ); // onserimento in testa
    insert_tail(&head , 20); // inserimento in cod
    insert_tail(&head , 8); // inserimento in cod
    insert_tail(&head , 12); // inserimento in cod
    insert_head(&head, 10 ); // onserimento in testa
    insert_tail(&head , 2); // inserimento in cod

    struct node *sup=print_list_front(&head);
    printf("\n");
    print_list_back(&sup);
return 0 ;
}
