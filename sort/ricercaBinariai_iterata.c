//Ricerca Binaria
#include <stdio.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
    int array[]={1,2,3,4,5,6,7,8,9,10,11,14,15,16,17,18,19,20};
    int n,min=0,max=(sizeof(array)/sizeof(int)),mid=max/2;
    int elem=max;
    printf("inserisci un numero: ");
    scanf("%d",&n);
    for(int i=0 ; i<elem ; i++){
    if(n == array[mid]){
        printf("trovato\n");
        return 0;
    }else{
        if(n >= array[mid])
            min = mid;
        else
            max = mid;
    }
      mid=(max+min)/2;
    }
printf("NON trovato\n");
return 0;
}
