#include "sprint1.h"
void testNullEnc(void){
    char *pchString = malloc(14*sizeof(char));
    pchString[13]='\0';
    int *piData = malloc(14*sizeof(int));
    PrintData(14,0,piData);
    CharToBytes(14,pchString,piData);
    PrintData(14,0,piData);

    int iSizeData = sizeof(&piData);

    int iSizeChar = sizeof(&pchString);

    int key128[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    int rgiIv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int *piEncData = Enc(0,16,14,rgiIv,key128,piData);
    int iRetSize = piEncData[0];
    char *pchEncString = (char *)malloc((iRetSize+1)*sizeof(char));
    BytesToChar(iRetSize,piEncData,pchEncString,1);
    pchEncString[iRetSize]='\0';


    printf("Encrypted Result is:\n%s\n",pchEncString);

    
    printf("Size of piData %d\nSize of pchString %d\n",iSizeData,iSizeChar);
}
void testRotArr(){
    printf("Test case for size greater than size of array\n");
    int rgiTest[] = {0x01,0x12,0x23,0x34,0x45};
    RotWord(rgiTest);
    PrintData(5,0,rgiTest);
    printf("Null test case\n");
    int *piTest=(int *)malloc(5*sizeof(int));
    PrintData(5,0,piTest);
    RotWord(piTest);
    PrintData(5,0,piTest);

    
}
void testSubByte(){
    printf("Original: %x Substituted: %x\n",-5,SubByte(-5));
}
int main(void){
    printf("%d",INT_MIN);
    //testNullEnc();
    //testRotArr();
    testSubByte();
    return 0;
}
