#include "sprint1.h"
int main(void){
    int c[] = {0,1,2,3,4,5};
    int x[6] = c;
    for(int i = 0;i<6;i++){
        c[i]=c[i]*2;
    }
    printf("C\n");
    for(int j = 0;j<6;j++){
        printf("%d ",c[j]);
    }
    printf("\nX\n");
    for(int k = 0;k<6;k++){
        printf("%d ",x[k]);
    }
    return 0;
}
