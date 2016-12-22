#include "sprint1.h"
int main(void){
    int *userKey;//=malloc(sizeof(int));
    int userKeyIterator = 0;
    int iBuff=0;

    printf("Please enter key");
    while(scanf("%02x",iBuff)>0){
        userKey=(int *)realloc(userKey,(userKeyIterator++)*sizeof(int));
        userKey[userKeyIterator-1]=iBuff;
    }
    for(int i=0;i<userKeyIterator;i++){
        printf("%02x ",userKey[i]);
    }
    printf("\n");
    free(userKey);
    return 0;
}
