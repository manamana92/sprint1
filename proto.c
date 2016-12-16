#include "sprint1.h"
int main(void){
    int c[] = {0x48,0x65,0x6c,0x6c,0x6f};
    char x[6];
    for(int i = 0;i<6;i++){
        x[i]=(char)c[i];
    }
    x[6]='\0';
    printf("C\n");
    for(int j = 0;j<6;j++){
        printf("%d ",c[j]);
    }
    printf("\nX\n");
    printf("%s",x);
    return 0;
}
