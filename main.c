#include "sprint1.h"
/**
  *  @brief Kernel entrypoint
  *
  *  @param argc Number of arguments
  *  @param argv pointer to the arguments
  *
  *  @return status code (0 for success 1 for error)
  *  
  *  Program recieve key size as a command line
  *  argument
  */
int main(int argc,char *argv[]){
    if(argc>2){
        printf("Too Many Arguments.\n");
        return 1;
    }else if(argc<2){
        printf("Insufficient Arguments.\n");
        return 1;
    }
    /**  @var chBuff Char used for dynamic user input string*/
    char chBuff;
    /**  @var pchString Pointer to char array for storing
      *       user input
      */
    char * pchString = malloc(sizeof(char));
    /**  @var pchEncString Pointer to char array for storing
      *       string representation of the encrypted data
      */
    char * pchEncString = malloc(sizeof(char));
    /**  @var iIterator Integer for iterating for user input*/
    int iIterator = 0;
    /**  @var rgiIv Integer array for Initialization Vector*/
    int rgiIv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    /**  @var piKey Pointer to integer array for storing a
      *       dynamically allocated key based on random values
      */
    int *piKey=malloc(sizeof(int));
    /**int userKeyIterator = 1;
    int iBuff=0;
    int key128[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    int key192[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    int key192[32]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};*/
    /**  @var iKeySize Integer to store the Key Size given by user*/
    int iKeySize = atoi(argv[1]);
    /**  @var iRetSize Integer representing the size of the
      *        encrypted data
      */
    int iRetSize = 0;
    /** switch for usable key sizes versus incorrect key sizes*/
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
            /**  @var piData Pointer to integer array for storing
              *       integer values of user defined string
              */
            int *piData = malloc(iIterator*sizeof(int));
            CharToBytes(iIterator,pchString,piData);
            /**  @var piOutEncData Pointer to store returned value
              *       from Enc()
              */
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
            printf("Decrypt Result? (y/n)\n");
            char c = getchar();
 
            if(c=='y'||c=='Y'){
                for(int i =0;i<iRetSize;i++){
                    piOutEncData[i]=piOutEncData[i+1];
                }
                /**
                  *  @var piPostDec Pointer to store returned value
                  *       from Dec()
                  */
                int *piPostDec=Dec(0,iKeySize,iRetSize,rgiIv,piKey,piOutEncData);
                /**  @var iPostDecSize Size of decrypted data*/
                int iPostDecSize=piPostDec[0];
                /**  @var pchDecString Pointer to Decrypted String*/
                char *pchDecString=malloc((iPostDecSize)*sizeof(char));
                BytesToChar(iPostDecSize,piPostDec,pchDecString,1);
                pchDecString[iPostDecSize-1]='\0';
                puts(pchDecString);
                free(piPostDec);
                free(pchDecString);
            }

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
