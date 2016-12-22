#include "sprint1.h"
int main(int argc,char *argv[]){
    if(argc>2){
        printf("Too Many Arguments.\n");
        return 1;
    }else if(argc<2){
        printf("Insufficient Arguments.\n");
        return 1;
    }
    char chBuff;
    char * pchString = malloc(sizeof(char));
    char * pchEncString = malloc(sizeof(char));
    int iIterator = 0;
    int rgiIv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int *piKey=malloc(sizeof(int));
    /**int userKeyIterator = 1;
    int iBuff=0;
    int key128[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    int key192[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    int key192[32]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};*/

    int iKeySize = atoi(argv[1]);
    int iRetSize = 0;
    switch(iKeySize){
        case 128:
        case 192:
        case 256:
            iKeySize=iKeySize/8;
            piKey = realloc(piKey,iKeySize*sizeof(int));
            for(int iInIterator = 0;iInIterator<iKeySize;iInIterator++){
                piKey[iInIterator]=rand()/(RAND_MAX/256+1);
            }
            printf("Enter Message\n");
            while((chBuff=getchar())!='\n'){
                pchString = (char *)realloc(pchString, sizeof(char));
                pchString[iIterator++]=chBuff; 
            }
            int *piData = malloc(iIterator*sizeof(int));
            CharToBytes(iIterator,pchString,piData);
            int *piOutEncData = Enc(0,iKeySize,iIterator,rgiIv,piKey,piData);
            iRetSize = piOutEncData[0];
            /**printData(iRetSize,1,piOutEncData);
            for(int i = 0;i<iRetSize;i++){
                piOutEncData[i]=piOutEncData[i+1];
            }*/
            pchEncString = realloc(pchEncString,(iRetSize+1)*sizeof(char));
            BytesToChar(iRetSize,piOutEncData,pchEncString,1);
            pchEncString[iRetSize]='\0';


            printf("Encrypted Result is:\n%s\nKey: ",pchEncString);
            PrintData(iKeySize,0,piKey);
            printf("IV: ");
            PrintData(16,0,rgiIv);
            /**printf("Decrypt Result? (y/n)\n");
            char c = getchar();
    
            if(c=='y'||c=='Y'){
                for(int i =0;i<iRetSize;i++){
                    piOutEncData[i]=piOutEncData[i+1];
                }

                int *pPostDec=dec(0,iKeySize,iRetSize,rgiIv,piKey,piOutEncData);
                int iPostDecSize=pPostDec[0];
                char *pDecString=malloc((iPostDecSize)*sizeof(char));
                bytesToChar(iPostDecSize,pPostDec,pDecString,1);
                pDecString[iPostDecSize-1]='\0';
                puts(pDecString);
                free(pPostDec);
                free(pDecString);
            }*/
            free(piOutEncData);

            break;
        default:
            printf("Incorrect key size");
            return 1;
            break;
    }
    
    
    free(piKey);
    free(pchString);
    free(pchEncString);
    return 0;
}
