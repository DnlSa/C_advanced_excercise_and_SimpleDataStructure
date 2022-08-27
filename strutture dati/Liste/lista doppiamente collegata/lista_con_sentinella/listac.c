

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "listac.h"

typedef struct node{ //definizione nodi lista con un campo prev , next , key
    int key;
    struct node *next;
    struct node *prev;
    unsigned char c[];
}node_t;

//funzione che copia n volte il targe
// imposto il nodo sentinella in cima alla lista
node_t *init_list(node_t **head)
{
    *head = malloc(sizeof(node_t)+1); // definisco il primo nodo
    (*head)->key = 0 ;
    (*head)->next = NULL; // lo punto in entrambi i versi a NULL
    (*head)->prev= NULL; // lo punto in entrambi i versi a NULL
    (*head)->c[0]= '\0';
    return (*head); // ritorno l indirizzo del nodo testa
}


// creo una procedur di insert non ordinata
node_t *insert_head(node_t **head , int key, char *string)
{
    node_t *new_node ; // creo un nuovo nodo
    size_t len = strlen(string)+1;
    new_node= malloc(sizeof(node_t)+len); // creo spazio per allocare il nodo
    if(!new_node) // programmazione difensiva se e andato a buon fine la malloc
        exit(-1);
    new_node->key=key;
    memcpy( new_node->c , string , len);
    node_t *current = *head;
    new_node->next = current->next; // eredito puntatore dal nodo sentinella
    current->next = new_node; //ricollego al nuovo nodo;
    new_node->prev = current; // eredito puntatore dal nodo sentinella

return new_node; // ritorna indirizzo nodo inserito
}

node_t *insert_tail(node_t **head , int key , char *string )
{

    node_t *new_node ; // creo un nuovo nodo
    size_t len= strlen(string)+1;
    new_node= malloc(sizeof(node_t)+len); // creo spazio per allocare il nodo

    if(!new_node) // programmazione difensiva se e andato a buon fine la malloc
        exit(-1);
    new_node->key=key;
    memcpy( new_node->c , string , len);
    node_t *current = *head;

    new_node->next = NULL; // LO PUNTO A NULL
    while(current ->next !=NULL) // scorro i nodi fino a giungere a null
        current = current->next;

    current->next = new_node; // current next che prima puntava  NULL  ADESSO PUNTA AL NUOVO NODO
     new_node->prev = current; // eredito puntatore dal nodo sentinella
    return new_node; // ritorna indirizzo nodo inserito
}



// procedura di stampa in lista se li passo subito dopo init_list non stampa nulla
void print_list(node_t *head)
{

    while(head->next != NULL){
        head = head->next;
        printf("il nodo ha chiave : %d , stringa : %s \n", head->key , head ->c );

    }

}
// procedura di eliminazione di un nodo generico in lista
void free_element(node_t **head , int data)
{
    node_t *current;
    node_t *prev;
    current = *head;
    current = current->next;
    prev = *head;
    while(current->key != data && current->next != NULL ){
        current = current->next;
        prev = prev->next;
    }

    if(current->key == data &&current->next == NULL){
        prev->next = NULL;
        current->prev = NULL;
        current->next = NULL;
        free(current);
        return;

    }else if (current->key != data && current->next == NULL){
        printf("nodo non presente in lista\n");
        return;

    }else{
        node_t *temp;
        temp = current;
        current = current->next;
        prev->next = current;
        current->prev = prev;
        temp->prev = NULL;
        temp->next = NULL;
        free(temp);
    }
}

//procedura di svuotamento completo della memoria

void free_list(node_t **head)
{

    while((*head)!= NULL){ // scorro la testa in avanti fino a quando non  diventa null
        node_t *temp; // nodo locale che conterra il nodo da cancellare

        temp = *head;
        (*head)= (*head)->next; // scorro il nodo
        free(temp); // libero il vecchio nodo non puntato piu
    }

}


