#include <stdio.h>
#include <stdlib.h>




typedef struct tree_node{

    int data;
    struct tree_node *parent;
    struct tree_node *leftChild;
    struct tree_node *rightChild;

}node_t;


node_t root = NULL;

node_t *min(node_t *T){

    while(T->leftChild != NULL)
        T = T->leftChild;
    return T;
}


node_t *max(node_t *T)
{
    while(T->rightChild != NULL)
            T=T->rightChild;
    return T;
}


node_t *search(int key)
{
    node_t current = root;



}



