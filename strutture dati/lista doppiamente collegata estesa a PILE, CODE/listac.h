#pragma once



typedef struct node node_t;//finizione nodi lista con un campo prev , next , key

node_t *init_list(node_t **head); // inizializzazione della lista
node_t *insert_head(node_t **head , int key,char *string); // inserimento in testa
node_t *insert_tail(node_t **head , int key,char *string); // inserimetno in coda
void print_list(node_t *head); // stampa lista
void free_element(node_t **head , int data); // elimina un elemento generico in lista
void free_list(node_t **head); // cancella tutta la lista elimino problemi di leak


// usate per lo stack puo fare le seguenti 3 operazioni
int pop(node_t **head); // elimina l ultimo elemento della lista
node_t *push(node_t **head , int key,char *string); // inserisce in coda alla lista
void top(node_t **head); // legge in cima alla list

// coda o QUEUE;
// e semplice le code possono inserire in testa e cancellare in coda
// altimenti posso inserire in coda e cancellare in testa
// come esempio definisco un inserimento in coda e una cancellazione in testa

node_t *enqueue(node_t **head , int key,char *string); // inseriemnto in coda
int dequeue(node_t **head);
