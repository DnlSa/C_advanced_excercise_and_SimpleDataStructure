/*
 note di funzionamento
 chiamare il comando

 ./test num.txt

 i numeri sono nel file verranno letti e processati
 */

//metodo alternativo per fare l esercizio
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// usare la union e molto piu elegante e si adotta qualcosa fatto da qualcun altro

union my_float{ // dichiaro un union
    struct std_IEEE754{ // tipo di dato struttura che contiene mantissa esponente segno
        uint32_t mantissa: 23;
        uint32_t esponente: 8;
        uint32_t segno: 1 ;
    }rapp_bin; // qua viene chiusa la struttura
    float f ;  // qua viene definito il numero float
};

uint32_t mantissa(float value){

    union my_float valore;
    valore.f = value ; // passo nella mia union il valore
    int m = valore.rapp_bin.mantissa ; // vado a prendere nella  mia struttura la mantissa
    return m;
}

typedef struct list {
    int counter;
    int key;
    struct list *next;
}node_t ;


// del resto farlo a mano fa capire che  si possono implementare tante cose e si manipola il bit stesso
int conv_32bit(float numero)
{
    int s ,m , e;
    unsigned int *ptr = (unsigned int *)&numero ; // ne prendo l indirizzo
    s = *ptr >> 31 ; // fit segno lo trovo shiftando di 31 bit a destra
    e = *ptr & 0x7f800000;
    e >>= 23 ; // si shifta di 23 bit tolgo la mantissa
    m = *ptr & 0x007fffff; // stessa cosa metto in and
    return m;
}
void init_list(node_t **head)
{
    (*head) = malloc(sizeof(node_t));
    (*head)->key = 0 ;
    (*head)->counter = 0 ;
    (*head)->next = NULL;
}

int scansiona(node_t **head, int val)
{
    node_t *cur = (*head);
    while (cur != NULL){
        if(cur->key == val){ // se trovo il matching incremtno di 1
            cur->counter +=1 ; // incremento counter
            return 1 ; // e ritorna 1 come segnale di uscita di insert
        }else
            cur = cur->next;
    }
return 0 ; // se 0 alora tocca allocare
}

void insert_node(struct list **head, float value)
{
      //  int val = mantissa(value);
    int val = conv_32bit(value); // costruisco una procedura di conversione del numero
      int p =  scansiona(&(*head), val);
      if(p)// se 1 allora ha trovato un nodo con la stessa chiave
        return;
      // se arrivo qui devo allocare il nodo;
      node_t *new;
      new = malloc(sizeof(struct list));
      if(new == NULL){
          printf("impossibile allocare spazio ");
          exit(EXIT_FAILURE);
      }
      new->key = val; // imposto il valore
      new->counter = 0 ; // imposto un contatore a 0

      if((*head)->next == NULL){
            new->next = NULL;
            (*head)->next = new;
            return;
      }
      // allocazione in testa
      node_t *cur = (*head); // definisco il nodo
      cur = cur->next;// lo punto al successivo cio e garantitto che la testa non punta  a NULL
       new->next = cur; // punto il nuovo nodo a curr
      (*head)->next = new; // punto la testa al nuovo nodo
}

// questa funzione  deve solo fare free del nodo
void remove_node(struct list **head, float value)
{
  //  int val = mantissa(value);
   int val = conv_32bit(value);
    if((*head)->next == NULL){
        printf("la lista e vuota");
        return;
    }
    node_t *cur = (*head);
    node_t *prev = (*head);
    cur = cur->next;
    while (cur != NULL)
        if(cur->key == val){
            node_t *temp = cur;
            if(cur->next ==NULL){
                prev->next = NULL;
                free(temp);
                return;
            }else{
                cur= cur->next;
                prev->next = cur;
                free(temp);
                return;
            }

        }else{
            cur= cur->next;
            prev = prev->next;
        }
    printf("nodo da eliminare non trovato\n");
}


void print(node_t *head)
{
    node_t *supervisor = head;
    supervisor=supervisor->next;
    while (supervisor != NULL){
        printf(" %d  , %d \n", supervisor->key , supervisor->counter);
        supervisor=supervisor->next;
    }
}
int main(int argc , char **argv){

    node_t *head= NULL;
    init_list(&head);
     FILE *file = fopen(argv[1],"r");
     float numero;
    while(fscanf( file , "%f", &numero )!= EOF){ // legge ogni linea da 256 caratteri
        insert_node(&head,numero);
    }
     print(head);
     fclose(file);
    return 0;
}
