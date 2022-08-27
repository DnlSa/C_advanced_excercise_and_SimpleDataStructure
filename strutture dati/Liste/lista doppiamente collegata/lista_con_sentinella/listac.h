#pragma once



typedef struct node node_t;//finizione nodi lista con un campo prev , next , key
node_t *init_list(node_t **head);
node_t *insert_head(node_t **head , int key,char *string);
node_t *insert_tail(node_t **head , int key,char *string);
void print_list(node_t *head);
void free_element(node_t **head , int data);
void free_list(node_t **head);




