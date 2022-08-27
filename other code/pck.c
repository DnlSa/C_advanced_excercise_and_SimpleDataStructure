 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define MAX_SIZE 4096


int main(int argc , char **argv){


    char buff_0[MAX_SIZE];
    char buff_1[MAX_SIZE];
    strcpy(buff_0, argv[1]);
    memcpy(buff_1 , argv[1], strlen(argv[1]));
    printf("buff_0 -> %s\n", buff_0); // funziona meglio
    printf("buff_1 -> %s\n", buff_1); // ci mette na p finale





}
