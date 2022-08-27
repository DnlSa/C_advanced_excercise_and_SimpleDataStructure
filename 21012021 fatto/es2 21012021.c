#include <stdio.h>

int main(void)
{
    int n=500,count=0;
    for(int i=1; i<=n ; i++)
        count+=i*i;
        if(count < 0){
            printf("overflow");
            return -1;
        }

    printf("%d\n",count);
return 0 ;

}
