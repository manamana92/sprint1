#include "sprint1.h"
void PrintData(int iSize,int iOffset,int * piData){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        printf("%02x ",piData[iOffset+iIterator]);
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
    int iBlockSize = 16;
    int rgiIn[iBlockSize];
    int rgiOut[iBlockSize];
    int iNb=4,iNr,iNk;

    /**Data Padding 80 00 ...*/
    int iDataSizeModBlock=(iDataSize+1)%iBlockSize;
    int iPaddedSize=iDataSize+(iBlockSize-iDataSizeModBlock)+1;
    int iNumBlocks = iPaddedSize/iBlockSize;
    /**printf("Data Size: %d, Mod Size: %d, Padded Size: %d",dataSize,iDataSizeModBlock,iPaddedSize);*/
    piData=(int *)realloc(piData,iPaddedSize*sizeof(int));
    piData[iDataSize]=0x80;
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
            break;
    }
    int rgrgiKeySchedule[iNb*(iNr+1)][4];
    KeyExpansion(iNb,iNr,iNk,rgiKey,rgrgiKeySchedule);


    for(int iOutIterator = 0;iOutIterator<iNumBlocks;iOutIterator++){
        for(int iInIterator0=0;iInIterator0<iBlockSize;iInIterator0++){
            rgiIn[iInIterator0]=piData[(iOutIterator*iBlockSize)+iInIterator0];
        }
        /**printf("Block %d",i);
        PrintData(iBlockSize,0,rgiIn);*/
        if(iOutIterator==0){
            XorVector(iBlockSize,rgiIn,rgiIv);
        }else{
            XorVector(iBlockSize,rgiIn,rgiOut);
        }
        Cipher(iNb,iNr,rgiIn,rgiOut,rgrgiKeySchedule);

        for(int iInIterator1=0;iInIterator1<iBlockSize;iInIterator1++){
            piEncData[(iOutIterator*iBlockSize)+iInIterator1+1]=rgiOut[iInIterator1];
        }

    }
    /**PrintData(iPaddedSize,piEncData);*/
    piEncData[0]=iPaddedSize;
    free(piData);
    return piEncData;
}
int *DecCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData){
    int iBlockSize = 16;
    int iNumBlocks=iDataSize/iBlockSize;
    int rgiIn[iBlockSize];
    int rgiOut[iBlockSize];
    int lastIn[iBlockSize];
    int iNb=4,iNr,iNk;
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
            break;
    }
    int rgrgiKeySchedule[iNb*(iNr+1)][4];
    KeyExpansion(iNb,iNr,iNk,rgiKey,rgrgiKeySchedule);

    int *piData = malloc((iDataSize+1)*sizeof(int));

    for(int iOutIterator = 0;iOutIterator<iNumBlocks;iOutIterator++){
        for(int iInIterator0 = 0;iInIterator0<iBlockSize;iInIterator0++){
            rgiIn[iInIterator0]=piEncData[(iOutIterator*iBlockSize)+iInIterator0];
        }
        InvCipher(iNb,iNr,rgiIn,rgiOut,rgrgiKeySchedule);
        
        if(iOutIterator==0){
            XorVector(iBlockSize,rgiOut,rgiIv);
            for(int iInIterator1=0;iInIterator1<iBlockSize;iInIterator1++){
                lastIn[iInIterator1]=rgiIn[iInIterator1];
            }
        }else{
            XorVector(iBlockSize,rgiOut,lastIn);
            for(int iInIterator2=0;iInIterator2<iBlockSize;iInIterator2++){
                lastIn[iInIterator2]=rgiIn[iInIterator2];
            }
        }

        for(int iInIterator3 = 0;iInIterator3<iBlockSize;iInIterator3++){
            piData[(iOutIterator*iBlockSize)+iInIterator3+1]=rgiOut[iInIterator3];
        }
    }
    PrintData(iDataSize,0,piData);
    /**Handle Padding 80 00 00 ...*/
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
    int *retVal;
    switch(iMode){
        case 0:
            retVal = EncCBC(iKeySize,iDataSize,rgiIv,rgiKey,piData);
            break;
        default:
	    break;
    }
    return retVal;
}
int *Dec(int iMode,int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData){
    int *retVal;
    switch(iMode){
        case 0:
            retVal = DecCBC(iKeySize,iDataSize,rgiIv,rgiKey,piEncData);
            printf("Outside Size: %d\n",iDataSize);
            break;
        default:
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
