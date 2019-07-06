#include "sprint1.h"
void PrintData(int iSize, int iOffset, unsigned char *puchData) {
    for (int iIterator = 0; iIterator < iSize; iIterator++) {
        printf("%02x ", puchData[iOffset + iIterator]);
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
void XorVector(int iSize,unsigned char rguchVectorPlain[iSize],unsigned char rguchVector[iSize]){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        rguchVectorPlain[iIterator] =
            rguchVectorPlain[iIterator] ^ rguchVector[iIterator];
    }
}
int EncCBC(unsigned char rguchIv[16], int iKeySize,
           unsigned char rguchKey[iKeySize], int iDataSize,
           unsigned char *puchData, unsigned char **puchEncData) {
    /**  @var  rgiIn Integer array for the input vector*/
    unsigned char rguchIn[BLOCKSIZE];
    /**  @var  rgiOut Integer array for the output vector*/
    unsigned char rguchOut[BLOCKSIZE];
    /**
      *  @var iNr The number of rounds. This is
      *       determined by the size of the key.
      *  @var iNk the number of words in the key.
      */
    int iNr, iNk;

    /**Data Padding 80 00 ...*/
    /**  @var iDataSizeModBlock Integer to store (iDataSize+1) mod BLOCKSIZE*/
    int iDataSizeModBlock = (iDataSize + 1) % BLOCKSIZE;
    /**  @var iPaddedSize Integer for the padded size*/
    int iPaddedSize = iDataSize + (BLOCKSIZE - iDataSizeModBlock) + 1;
    /**  @var iNumBlocks Integer for the number of blocks*/
    int iNumBlocks = iPaddedSize / BLOCKSIZE;
    /**printf("Data Size: %d, Mod Size: %d, Padded Size: %d",dataSize,iDataSizeModBlock,iPaddedSize);*/
    unsigned char *puchPaddedData = (unsigned char *)calloc(iPaddedSize, 1);
    unsigned char *puchEnc = (unsigned char *)calloc(iPaddedSize, 1);
    memcpy(puchPaddedData, puchData, iDataSize);
    puchPaddedData[iDataSize] = 0x80;
    memset(puchPaddedData + iDataSize + 1, 0, iPaddedSize);
    PrintData(iPaddedSize, 0, puchPaddedData);
    /**  @var Pointer to integer array for storing Encrypted Data*/
//    int *piEncData=(int *)malloc((iPaddedSize+1)*sizeof(int));
//    for(int iIterator = iDataSize+1;iIterator<iPaddedSize;iIterator++){
//        piData[iIterator]=0;
//    }
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
            return 0;
            break;
    }
    /**  @var rgrgiKeySchedule 2-D integer array for storing Key Schedule*/
    unsigned char rgrguchKeySchedule[NB * (iNr + 1)][4];
    KeyExpansion(iNr, iNk, rguchKey, rgrguchKeySchedule);

    for (int iOutIterator = 0; iOutIterator < iNumBlocks; iOutIterator++) {
        for (int iInIterator0 = 0; iInIterator0 < BLOCKSIZE; iInIterator0++) {
            rguchIn[iInIterator0] =
                puchPaddedData[(iOutIterator * BLOCKSIZE) + iInIterator0];
        }
        /**printf("Block %d",i);
        PrintData(BLOCKSIZE,0,rgiIn);*/
        if (iOutIterator == 0) {
            XorVector(BLOCKSIZE, rguchIn, rguchIv);
        } else {
            XorVector(BLOCKSIZE, rguchIn, rguchOut);
        }
        Cipher(iNr, rguchIn, rguchOut, rgrguchKeySchedule);

        for (int iInIterator1 = 0; iInIterator1 < BLOCKSIZE; iInIterator1++) {
            puchEnc[(iOutIterator * BLOCKSIZE) + iInIterator1] =
                rguchOut[iInIterator1];
        }
    }
    /**PrintData(iPaddedSize,piEncData);*/
    *puchEncData = puchEnc;
    free(puchPaddedData);
    return iPaddedSize;
}
int DecCBC(unsigned char rguchIv[16], int iKeySize,
           unsigned char rguchKey[iKeySize], int iDataSize,
           unsigned char *puchEncData, unsigned char **puchDecData) {
    /**  @var iNumBlocks Integer for the number of blocks*/
    int iNumBlocks=iDataSize/BLOCKSIZE;
    /**  @var  rgiIn Integer array for the input vector*/
    unsigned char rguchIn[BLOCKSIZE];
    /**  @var  rgiOut Integer array for the output vector*/
    unsigned char rguchOut[BLOCKSIZE];
    /**  @var  rgiLastIn Integer array for the last input vector*/
    unsigned char rguchLastIn[BLOCKSIZE];
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
            return 0;
            break;
    }
    /**  @var rgrgiKeySchedule 2-D integer array for storing Key Schedule*/
    unsigned char rgrguchKeySchedule[NB*(iNr+1)][4];
    KeyExpansion(iNr, iNk, rguchKey, rgrguchKeySchedule);

    /**  @var Pointer to integer array for storing Decrypted Data*/
    unsigned char *puchData = malloc((iDataSize) * sizeof(unsigned char));

    for (int iOutIterator = 0; iOutIterator < iNumBlocks; iOutIterator++) {
        for (int iInIterator0 = 0; iInIterator0 < BLOCKSIZE; iInIterator0++) {
            rguchIn[iInIterator0] =
                puchEncData[(iOutIterator * BLOCKSIZE) + iInIterator0];
        }
        InvCipher(iNr, rguchIn, rguchOut, rgrguchKeySchedule);

        if(iOutIterator==0){
            XorVector(BLOCKSIZE, rguchOut, rguchIv);
            for (int iInIterator1 = 0; iInIterator1 < BLOCKSIZE;
                 iInIterator1++) {
                rguchLastIn[iInIterator1] = rguchIn[iInIterator1];
            }
        }else{
            XorVector(BLOCKSIZE, rguchOut, rguchLastIn);
            for(int iInIterator2=0;iInIterator2<BLOCKSIZE;iInIterator2++){
                rguchLastIn[iInIterator2] = rguchIn[iInIterator2];
            }
        }

        for (int iInIterator3 = 0; iInIterator3 < BLOCKSIZE; iInIterator3++) {
            puchData[(iOutIterator * BLOCKSIZE) + iInIterator3] =
                rguchOut[iInIterator3];
        }
    }
    /**Handle Padding 80 00 00 ...*/
    /** @var iLastByte Integer for finding the last byte of the
      *       Decrypted data
      */
    int iLastByte = iDataSize - 1;
    while (puchData[iLastByte] != 0x80 && iLastByte > 0) {
        iLastByte--;
    }
    *puchDecData = (unsigned char *)calloc(iLastByte + 1, sizeof(unsigned char));
    memcpy(*puchDecData, puchData, iLastByte);
    return iLastByte;
}
int Enc(int iMode,unsigned char rguchIv[16],int iKeySize,unsigned char rguchKey[iKeySize],int iDataSize, unsigned char *puchData, unsigned char **puchEncData){
    /**  @var retVal Pointer for storing returned value from
      *       encryption function
      */
    int retVal = -1;
    switch(iMode){
        case 0:
            retVal = EncCBC(rguchIv, iKeySize, rguchKey, iDataSize, puchData,
                            puchEncData);
            break;
        default:
            break;
    }
    return retVal;
}
int Dec(int iMode, unsigned char rguchIv[16], int iKeySize,
        unsigned char rguchKey[iKeySize], int iDataSize,
        unsigned char *puchEncData, unsigned char **puchDecData) {
    /**  @var retVal Pointer for storing returned value from
      *       encryption function
      */
    int retVal = -1;
    switch(iMode){
        case 0:
            retVal = DecCBC(rguchIv, iKeySize, rguchKey, iDataSize, puchEncData,
                            puchDecData);
            break;
        default:
            break;
    }
    return retVal;
}
