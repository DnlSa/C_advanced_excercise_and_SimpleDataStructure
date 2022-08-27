#include <stdio.h>
#include <string.h>

int main(int argc , char *argv[])
{
    int n1= strlen(argv[1]),min, MAX,n2= strlen(argv[2]),count=0;
    if(n1<n2){
        min=n1;
        MAX=n2;
    }else{
        min=n2;
        MAX=n1;
    }
    char string2[min];
    for(int i=0 ; i<min ; i++){
        if(argv[1][i]==argv[2][i]){
            string2[count]=argv[2][i];
            count++;
        }else
            ;
    }
    if(count!=0)
        printf("%s\n", string2);
    else
        printf("nessuna lettera in comune\n");
return 0 ;
}
