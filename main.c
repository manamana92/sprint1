#include "sprint1.h"
int main(void){
    char cBuff;
    char * pString = malloc(sizeof(char));
    printf("Enter integer\n");
    int iterator = 0;
    while((cBuff=getchar())!='\n'){
        realloc(pString, sizeof(char));
        pString[iterator++]=cBuff; 
    }
    printf("%s\n",pString);
    free(pString);
    return 0;
}
