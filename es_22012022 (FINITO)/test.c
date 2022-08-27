 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 10 // VITE



typedef struct node {

    int n ; // numero di bonus
    char *word;
    struct node * next ;
    unsigned char c[];
}node_t;


void insert_node(node_t **head , char *string , int n );
void init_list (node_t **head );
void wrapper(char *path , node_t **head);
void print(node_t *head);



 // algoritmo di eliminazione del nodo tramite stringa
void free_element(node_t **head, char *string)
{
    node_t *current = (*head);
    node_t *prev = (*head);
    current = current->next;  // punto al primo nodo
    while (current != (*head)){

        if(!(strcmp(current->c , string))){ // strcmp e 0 se le 2 stringhe sono uguali
            node_t *temp = current;
            current = current->next ;
            prev->next = current;
            free(temp);
            return ;
        }else{
            prev = prev->next;
            current = current->next ;
        }
    }
}
void init_list (node_t **head )
{
    (*head) = malloc(sizeof(node_t)+1);
    (*head)-> next = NULL ;
    (*head)-> n = N ;
    (*head)-> c[0]= '\0';
}

void wrapper(char *path , node_t **head)
{
    char c ;
    int i=0 , n;
    char string[1024];
    FILE *fp = fopen(path , "r");
    while((c= fgetc(fp))!= EOF){
        if(c == ';'){
            string[i++]= '\0';
            c= fgetc(fp) ;
            n = (int)c-48 ;
            insert_node(&(*head), string , n);
            i=0;
        }else{
            string[i]= c;
            i++;
        }
    }
}
// inserimenti in coda sempre
void insert_node(node_t **head , char *string , int n )
{
    node_t *new =  0 ;
    node_t *current = *head;
    int len = strlen(string)+1;
    new = malloc(sizeof(node_t)+len );
    if(!new)
        exit(-1);

    memcpy(new->c , string ,len);
    new->n = n ;
    if(current->next == NULL){
        current->next = new;
        new->next = (*head);
        return;
    }
    while(current->next != (*head)){
           current = current->next;
    }
    new->next = (*head);
    current->next = new;
}

void print(node_t *head)
{
     node_t *supervisor = head;
     supervisor = supervisor->next;
        while (supervisor != head){
            printf("%s || %d \n" , supervisor->c , supervisor->n);
            supervisor = supervisor->next;
        }
}
// l algoritmo non alloca nulla ma lavora tutto in place .
// dove dalla parola stampa gli undwer score poi se indovino la lettera li converte in upper e passa avanti
void impiccato(node_t **head){
    node_t *turno = (*head);
    int count = 0 ;
    turno = turno->next ; // punta alla prima parola
    while ((*head)->n > 0 && count <= 3){ // condizione di uscita del gioco
        if(turno == (*head)){
             turno = turno->next ;
        }
        int i=0; // reset dell indica
        // stampa la parola
        while (turno->c[i] != '\0')   // stampa la parola
        {
            if((turno->c[i]) > 90 ){
                printf(" _ ");
                i++;
            }else{
                printf(" %c ", turno->c[i]);
                i++;
            }
        }
         printf("\n inserisci una lettera: ");
         // imparare a memoria questo modo perche funziona bene
        char input = (char)tolower((char)getchar()); //parte interessante OGNI INPUT CHE PASSO E PASSATO IN MAIUSCOLO
                if(input != '\n')
                   while(getchar() != '\n');

        int flag= 0;
        int lunghezza = strlen(turno->c);

        for(int j= 0 ; j <= lunghezza; j++){ // si controlla tutta la parola
            if(turno->c[j] == input){
                 char c = turno->c[j];
                 turno->c[j]= c-32;
                flag = 1;
            }
        }
        if(flag){
            printf("lettera indovinata , ti rimangono : %d\n", (*head)->n);
        }else{
            (*head)->n -= 1 ;
            printf("hai sbagliato ti rimangono : %d\n", (*head)->n);;
            }
        int p= 0;
        for(int j= 0 ; j <= lunghezza; j++){
            if(turno->c[j]<97)
                p++;

            else
                p--;
        }
        if(p>=lunghezza){

            (*head)->n += turno->n ;
            printf("\n complimenti hai completato la parola hai numero di vite : %d \n", (*head)->n);
            free_element(&(*head), turno->c); // elimina la parola dalla lista dalla stringa
            count++;
        }
             turno = turno->next;
    }
if(((*head)->n != 0))
     printf("\nhai vinto \n");
else
     printf("\nhai perso\n");
}
int main(int argc , char ** argv){
        node_t *head = NULL ;
        init_list(&head);
        wrapper(argv[1], &head);
       // print(head);
        impiccato(&head);
        return 0 ;
}
