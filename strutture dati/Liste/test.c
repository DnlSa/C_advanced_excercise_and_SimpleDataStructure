#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node {
    int data ;
    struct node *next;
    struct node *prev;

}node_t;


typedef struct list {
    node_t *testa;
    node_t *coda;


}list_t;



bool empty(node_t *head )
{
    return( (*head)==NULL);
}









int main(void)
{
    node_t *head=NULL;
    list_t *testa=NULL;
    list_t *coda = NULL;










}
