 
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "listac.h"

struct node *head_1; //definizione nodi lista con un campo prev , next , key
int main(void)
{
    init_list(&head_1);
    insert_head(&head_1 , 1 , "ciao");
    insert_head(&head_1 , 2 , "mamma");
    insert_tail(&head_1 , 3 , "guarda");
    insert_tail(&head_1 , 4 , "come");
    insert_head(&head_1 , 5 , "mi");
    insert_head(&head_1 , 6 , "diverto");
    print_list(head_1);
    push(&head_1 , 45 , "push");
    push(&head_1 , 34 , "push");
     printf("\n");
    pop (&head_1);
    dequeue(&head_1);
    enqueue(&head_1 , 78 , "enqueue");
    print_list(head_1);
    printf("\n");
    top(&head_1);
    free_list(&head_1);
return 0 ;
}
