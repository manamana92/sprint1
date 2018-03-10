#include "sprint1.h"
void PrintData(int iSize,int iOffset,unsigned char *puchData){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        printf("%02x ",puchData[iOffset+iIterator]);
    }
    printf("\n");
}
int CharToBytes(int iSize,char * pchArr,int * piBytes){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        piBytes[iIterator]=(int)pchArr[iIterator];
    }

    return 0;
}
int BytesToChar(int iSize,int * piBytes,char * pchArr,int iOffset){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        pchArr[iIterator]=(char)piBytes[iOffset+iIterator];
    }
    return 0;
}
void FillIn(int iSize,int iOffset,int rgiIn[iSize],int * piData){
    for(int iIterator=0;iIterator<iSize;iIterator++){
        rgiIn[iIterator]=piData[iOffset+iIterator];
    }
}
void XorVector(int iSize,int rgiVectorPlain[iSize],int rgiVector[iSize]){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        rgiVectorPlain[iIterator]=rgiVectorPlain[iIterator]^rgiVector[iIterator];
    }
}
int *EncCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piData){
    /**  @var  rgiIn Integer array for the input vector*/
    int rgiIn[BLOCKSIZE];
    /**  @var  rgiOut Integer array for the output vector*/
    int rgiOut[BLOCKSIZE];
    /**
      *  @var iNr The number of rounds. This is
      *       determined by the size of the key.
      *  @var iNk the number of words in the key.
      */
    int iNr,iNk;

    /**Data Padding 80 00 ...*/
    /**  @var iDataSizeModBlock Integer to store (iDataSize+1) mod BLOCKSIZE*/
    int iDataSizeModBlock=(iDataSize+1)%BLOCKSIZE;
    /**  @var iPaddedSize Integer for the padded size*/
    int iPaddedSize=iDataSize+(BLOCKSIZE-iDataSizeModBlock)+1;
    /**  @var iNumBlocks Integer for the number of blocks*/
    int iNumBlocks = iPaddedSize/BLOCKSIZE;
    /**printf("Data Size: %d, Mod Size: %d, Padded Size: %d",dataSize,iDataSizeModBlock,iPaddedSize);*/
    piData=(int *)realloc(piData,iPaddedSize*sizeof(int));
    piData[iDataSize]=0x80;
    /**  @var Pointer to integer array for storing Encrypted Data*/
    int *piEncData=(int *)malloc((iPaddedSize+1)*sizeof(int));
    for(int iIterator = iDataSize+1;iIterator<iPaddedSize;iIterator++){
        piData[iIterator]=0;
    }
    switch(iKeySize){
        case 16:
            iNr = 10;
            iNk = 4;
            break;
        case 24:
            iNr = 12;
            iNk = 6;
            break;
        case 32:
            iNr = 14;
            iNk = 8;
            break;
        default:
            return NULL;
            break;
    }
    /**  @var rgrgiKeySchedule 2-D integer array for storing Key Schedule*/
    int rgrgiKeySchedule[NB*(iNr+1)][4];
    KeyExpansion(iNr,iNk,rgiKey,rgrgiKeySchedule);


    for(int iOutIterator = 0;iOutIterator<iNumBlocks;iOutIterator++){
        for(int iInIterator0=0;iInIterator0<BLOCKSIZE;iInIterator0++){
            rgiIn[iInIterator0]=piData[(iOutIterator*BLOCKSIZE)+iInIterator0];
        }
        /**printf("Block %d",i);
        PrintData(BLOCKSIZE,0,rgiIn);*/
        if(iOutIterator==0){
            XorVector(BLOCKSIZE,rgiIn,rgiIv);
        }else{
            XorVector(BLOCKSIZE,rgiIn,rgiOut);
        }
        Cipher(iNr,rgiIn,rgiOut,rgrgiKeySchedule);

        for(int iInIterator1=0;iInIterator1<BLOCKSIZE;iInIterator1++){
            piEncData[(iOutIterator*BLOCKSIZE)+iInIterator1+1]=rgiOut[iInIterator1];
        }

    }
    /**PrintData(iPaddedSize,piEncData);*/
    piEncData[0]=iPaddedSize;
    free(piData);
    return piEncData;
}
int *DecCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData){
    /**  @var iNumBlocks Integer for the number of blocks*/
    int iNumBlocks=iDataSize/BLOCKSIZE;
    /**  @var  rgiIn Integer array for the input vector*/
    int rgiIn[BLOCKSIZE];
    /**  @var  rgiOut Integer array for the output vector*/
    int rgiOut[BLOCKSIZE];
    /**  @var  rgiLastIn Integer array for the last input vector*/
    int rgiLastIn[BLOCKSIZE];
    /**
      *  @var iNr The number of rounds. This is
      *       determined by the size of the key.
      *  @var iNk the number of words in the key.
      */
    int iNr,iNk;
    switch(iKeySize){
        case 16:
            iNr = 10;
            iNk = 4;
            break;
        case 24:
            iNr = 12;
            iNk = 6;
            break;
        case 32:
            iNr = 14;
            iNk = 8;
            break;
        default:
            return NULL;
            break;
    }
    /**  @var rgrgiKeySchedule 2-D integer array for storing Key Schedule*/
    int rgrgiKeySchedule[NB*(iNr+1)][4];
    KeyExpansion(iNr,iNk,rgiKey,rgrgiKeySchedule);

    /**  @var Pointer to integer array for storing Decrypted Data*/
    int *piData = malloc((iDataSize+1)*sizeof(int));

    for(int iOutIterator = 0;iOutIterator<iNumBlocks;iOutIterator++){
        for(int iInIterator0 = 0;iInIterator0<BLOCKSIZE;iInIterator0++){
            rgiIn[iInIterator0]=piEncData[(iOutIterator*BLOCKSIZE)+iInIterator0];
        }
        InvCipher(iNr,rgiIn,rgiOut,rgrgiKeySchedule);
        
        if(iOutIterator==0){
            XorVector(BLOCKSIZE,rgiOut,rgiIv);
            for(int iInIterator1=0;iInIterator1<BLOCKSIZE;iInIterator1++){
                rgiLastIn[iInIterator1]=rgiIn[iInIterator1];
            }
        }else{
            XorVector(BLOCKSIZE,rgiOut,rgiLastIn);
            for(int iInIterator2=0;iInIterator2<BLOCKSIZE;iInIterator2++){
                rgiLastIn[iInIterator2]=rgiIn[iInIterator2];
            }
        }

        for(int iInIterator3 = 0;iInIterator3<BLOCKSIZE;iInIterator3++){
            piData[(iOutIterator*BLOCKSIZE)+iInIterator3+1]=rgiOut[iInIterator3];
        }
    }
    /**PrintData(iDataSize,0,piData);
    Handle Padding 80 00 00 ...*/
    /** @var iLastByte Integer for finding the last byte of the
      *       Decrypted data
      */
    int iLastByte = iDataSize-1;
    while(piData[iLastByte]!=0x80&&iLastByte>0){
        iLastByte--;
    }
    piData=(int *)realloc(piData,(iLastByte+2)*sizeof(int));
    piData[0]=iLastByte;
    free(piEncData);
    return piData;
}
int *Enc(int iMode,int iKeySize,int iDataSize, int rgiIv[16],int rgiKey[iKeySize],int * piData){
    /**  @var retVal Pointer for storing returned value from
      *       encryption function
      */
    int *retVal;
    switch(iMode){
        case 0:
            retVal = EncCBC(iKeySize,iDataSize,rgiIv,rgiKey,piData);
            break;
        default:
            return NULL;
            break;
    }
    return retVal;
}
int *Dec(int iMode,int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData){
    /**  @var retVal Pointer for storing returned value from
      *       encryption function
      */
    int *retVal;
    switch(iMode){
        case 0:
            retVal = DecCBC(iKeySize,iDataSize,rgiIv,rgiKey,piEncData);
            break;
        default:
            return NULL;
            break;
    }
    return retVal;
}
/**int main(void){
    char *pchArr = "Hello my name is Chris";
    int sizeOfData = strlen(pchArr);
    int *piData=(int *)malloc(sizeOfData*sizeof(int));
    CharToBytes(sizeOfData,pchArr,piData);
    int rgiIv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int rgiKey[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

    int sizeOfBlock=16;

    int sizeOfDataModBlock = sizeOfData%sizeOfBlock;
    int iPaddedSize = sizeOfData+(sizeOfBlock-sizeOfDataModBlock);

    printf("Size Block: %d, Size Data: %d,Modded Size %d, Padded Size: %d\n",sizeOfBlock,sizeOfData,sizeOfDataModBlock,iPaddedSize);
    int *piEncData=(int *)malloc(iPaddedSize*sizeof(int));
    if(sizeOfDataModBlock!=0){
    PrintData(iPaddedSize,piEncData);
        piData = (int *)realloc(piData,iPaddedSize*sizeof(int));
	piData[sizeOfData]=0x80;
        for(int i = sizeOfData+1;i<iPaddedSize;i++){
            piData[i]=0;
        }
    }
    int *piEncData=(int *)malloc(sizeOfData*sizeof(int));
    int *outEncData = Enc(0,sizeOfBlock,sizeOfData,rgiIv,rgiKey,piData);
    int retSize = outEncData[0];
    printf("Length Enc Data: %d\n",retSize);
    PrintData(outEncData[0],outEncData);
    for(int i = 0;i<retSize;i++){
        outEncData[i]=outEncData[i+1];
    }
    outEncData = realloc(outEncData,retSize*sizeof(int));
    int *postDec= Dec(0,sizeOfBlock,retSize,rgiIv,rgiKey,outEncData);
    int postSize = postDec[0];
    int iLastByte = iPaddedSize-1;
    while(postDec[iLastByte]!=0x80&&iLastByte>0){
        iLastByte--;
    }
    printf("\n%02x\n",postDec[iLastByte]);

    postDec=(int *)realloc(postDec,(iLastByte+1)*sizeof(int));
    int sizeOut = sizeof(*postDec)/sizeof(int);
    printf("After Dec\n");
    PrintData(postSize+1,postDec);
    char *postArr=malloc((postDec[0])*sizeof(char));
    BytesToChar(postSize,postDec,postArr,1);
    postArr[postSize-1]='\0';
    printf("Size of char pchArr %d\n",postSize);
    puts(pchArr);
    puts(postArr);
    free(pchArr);
    free(outEncData);
    free(postDec);
    free(postArr);
}*/
