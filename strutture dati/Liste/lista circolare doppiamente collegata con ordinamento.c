
/*
Esercizio 2
Data una struttura dati lista, secondo lo schema riportato nel file esercizio2.c, implementare una funzione che
ordini i nodi della lista per chiavi crescenti.
Se necessario, è possibile popolare (per motivi di test) la lista con nodi arbitrari all'interno della funzione main().

*/


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

// vediamo di usare il bubble sort come metodo di ordinamento

void sorted_list(node_t **head)
{
    int swapped = 1;

while(swapped){
    swapped = 0 ; // imposto in false
    node_t *C = *head;
    C = C->next ; // punto al nodo 1 della lista che devo ordinare
    // scambia su tutta la lista
    while (C->next != (*head)){ // finche non rifaccio il giro
        if(C->key > C->next->key){
            printf("swapp\n");
            node_t *B = C ; // credo il nodo prec
            C= C->next ;
            node_t *A = B->prev ;
            node_t *D = C->next;
            swapped = 1 ;
        // situazione A B C D -> devo arrivare ad avere A C B D
            D->prev = B;
            C->prev = A;
            B->prev = C;
            A->next = C;
            B->next = D;
            C->next = B;
        }
        else{
            C = C->next ;
            }
        }
    }
}


int main(void)
{
    node_t *head = NULL  ;
     init_list(&head);
     insert_tail(&head , 5, "ciao");
     insert_tail(&head , 6, "amici");
     insert_tail(&head , 8, "va");
     insert_head(&head , 23 ,"tutto" );
     insert_head(&head , 24 ,"bene" );
    insert_tail(&head , 7, "come");
    sorted_list(&head);

    print(head);


    free_list(&head);
    return 0;
}
