#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct node{
        int key;
        struct node *next;
        struct node *prev;
        unsigned char  c[];
}node_t;


/*
 ////////////// LISTA CIRCOLARE //////////////////////////
                nodo 1 sentinella                       roba che serve
 TESTA -> |KEY|STRINGA|PUNTATORE A NEXT| -> |KEY|STRINGA|PUNTATORE A NEXT| -> |KEY|STRINGA|PUNTATORE A NEXT| -> puntatore al nodo 1 sentinella
 */

// la testa sara la mia sentinella
void init_list(node_t **head)
{
    (*head)= malloc(sizeof(node_t)+1);
    if(!(*head))
        exit(-1);
    (*head)-> key = 0 ;
    (*head)-> next = NULL;
    (*head)-> prev = NULL;
    (*head)-> c[0]= '\0';
}

// procedura di inserimento in coda
node_t  *insert_tail(node_t **head , int key, char *string)
{
    node_t *new_node; // definisco un nuovo nodo
    size_t len = strlen(string)+1 ;
    new_node = malloc (sizeof(node_t)+len); // do una dimensione del nodo
    if(!new_node)
        exit(-1);
    memcpy(new_node->c , string, len) ;  // INSERISCO LA STRINGA NEL CAMPO ESATTO
    new_node->key = key ; // INSERISCO LA CHIAVE

    node_t *current = (*head); // creo un nodo che punto alla testa
    new_node->next = (*head); // L'ultimo nodo punta sempre alla testa per definizione di lista circolare
    current->prev = new_node ; // punto il prev alla coda
    if((*head)->next == NULL){
         current->next = new_node;
         return new_node; // ritorna il nodo corrente
    }
    else {
    current= current->next; // punta ad un nodo nuovo
    while (current->next != (*head)){ // scorre nodi della lista in avanti
        current = current->next;
    }
    current->next = new_node;
    return new_node; // ritorna il nodo corrente
    }
}

void insert_head(node_t **head , int key , char *string )
{
    if((*head)->next == NULL){
        insert_tail(&(*head) , key , string);
        return;
    }
    node_t *new_node ;
    size_t len= strlen(string)+1;
    new_node = malloc(sizeof(node_t)+len); // alloco la grandezza della struttura + la lunghezza della stringa + terminatore di stringa
    if(!new_node)
        exit(-1); // uscita con segnale erore di memori

    // else
    new_node->key = key ;
    memcpy(new_node->c , string, len); // strcpy prende 2 puntatori destinazione e sorgente
    node_t *curr = (*head); // punta al nodo successivo
    curr = curr->next; // riferimento al primo nodo allocat
    (*head)->next = new_node;
    new_node->prev = (*head);
    new_node->next = curr;
    curr->prev = new_node;
    return;
}

// la print ne passo solo un puntatore cosi da poter
void print(node_t *head)
{
    node_t *supervisor = head;
    supervisor = supervisor->next; //punta al primo nodo allocato
    while (supervisor != head){
        printf("il nodo allocato e : %d , %s , %p\n", supervisor->key , supervisor->c , supervisor );
        supervisor = supervisor->next ;
    }
}

void free_list(node_t **head){
     // modificare la coda in modo da scollegarla
    node_t *curr = *head;
    curr = curr->prev;
    curr->next = NULL;
    while((*head)!= NULL){
        node_t *temp;
        temp = *head;
        (*head)=(*head)->next;
        free(temp);
    }
}

//  elimina nodo per valore key
void free_element(node_t **head , int n )
{
    node_t *current  = (*head) ; // punta a quello avanti
    node_t *preveus = (*head); // punta a quello dietro
    current = current->next;
    if(current == NULL){ // se non ho nodi in lista
        printf("la lista e vuota\n ");
        return;
    }
    if(current != NULL && current->next == (*head) && current->key == n)
    {
        node_t *temp = current;
        (*head)-> next = NULL;
        (*head)-> prev = NULL;
        free(temp);
        return;
    }
    while (current != (*head)){
        if(current->key == n){
            node_t *temp = current ;
            current = current->next; // lo faccio avanzare
            preveus->next= current ;  // scollego temp
            current->prev = preveus;
            free(temp); // libero
            return;
        }else{
            current = current->next;
            preveus = preveus->next;
        }
    }
    printf("Nodo non presente in lista\n");
}

void sorted_list(node_t **head)
{   int swapped = 1;

    node_t *current = (*head);
    node_t *preveus =(*head);
    current = current->next ; // punta al secondo nodo
    if(current == NULL){
        printf("lista vuota ");
        return;
    }
// il primo e la sentinella da evitare
    current = current->next ; // terzo nodo
    preveus = preveus->next ; // secondo nodo
    node_t *temp = current; // punta al target di comparazione
    current = current->next ; // punta al successivo
    if(current == (*head))
        return;
    if(temp->key > current->key){
            temp->next = current->next;
            temp->prev = current ;
            current->next = temp ;
            current->prev = preveus;
            preveus->next = current;
            sorted_list(&(*head)); // richiama la funzione
    }else
        while (swapped)
            if(temp->key < current->key)
                //////////////////////////////////////// FINIRE
}

int main(void)
{
    node_t *head = NULL  ;
     init_list(&head);
     insert_tail(&head , 5, "ciao ");
     insert_tail(&head , 6, "amici ");
     insert_tail(&head , 8, "come  ");
     insert_head(&head , 23 ,"pippo" );
     insert_head(&head , 24 ,"topolino" );
     free_element(&head , 50 );
    insert_tail(&head , 7, "va ");
    print(head);
    free_list(&head);
    return 0;
}
