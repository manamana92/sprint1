#include "sprint1.h"
void PrintState(unsigned char rgrguchState[4][NB]){
    for(int iOutIterator = 0;iOutIterator<4;iOutIterator++){
        for(int iInIterator = 0;iInIterator<NB;iInIterator++){
            printf("%02x ",rgrguchState[iOutIterator][iInIterator]);
        }
        printf("\n");
    }
    printf("\n");
}
void PrintDataCi(int iSize,unsigned char *puchData){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        printf("%02x ",puchData[iIterator]);
    }
    printf("\n");
}
void PrintStateVector(unsigned char rgrguchState[4][NB]){
    for(int iIterator = 0;iIterator<NB;iIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            printf("%02x ",rgrguchState[iInIterator][iIterator]);
        }
	printf("\n");
    }
    printf("\n");
}
void Reverse(unsigned char *puchArr, int iSize) {
    /**  @var iBuff Integer for storing value of piArr[] for swapping*/
    unsigned char uchBuff = 0;
    for(int iIterator = 0;iIterator<(iSize/2);iIterator++){
        uchBuff = puchArr[iIterator];
        puchArr[iIterator]=puchArr[iSize-iIterator-1];
        puchArr[iSize-iIterator-1]=uchBuff;
    }
}
int Mult(unsigned char uchX, unsigned char uchY){
    /**  @var iArrSize Size of puchBits*/
    int iArrSize = 8;
    /**  @var puchBits Pointer to integer array for storing bit
      *       representation of uchX
      */
    unsigned char *puchBits = (unsigned char *)malloc(iArrSize * sizeof(unsigned char));
    puchBits[0]=uchX&1;
    uchX=uchX>>1;
    /**  @var iCount Integer for Iterating through puchBits*/
    int iCount = 1;
    while(uchX>0){
        puchBits[iCount]=uchX&1;
        uchX=uchX>>1;
        iCount++;
    }
    Reverse(puchBits,iArrSize);
    /**  @var iRetVal Integer to be returned*/
    int iRetVal = 0;
    for(int iIterator=0;iIterator<iArrSize;iIterator++){
        if(iRetVal){
             iRetVal*=2;
             if(iRetVal&0x100){
                 iRetVal = iRetVal^0x011b;
             }
        }
        if(puchBits[iIterator]){
            iRetVal = iRetVal^uchY;
        }
    }
    free(puchBits);
    return iRetVal;
}

void InToState(unsigned char *puchIn, unsigned char rgrguchState[4][NB]) {
    /**  @var iVectorIterator Integer to iterate through puchIn*/
    int iVectorIterator = 0;
    for(int iOutIterator = 0;iOutIterator<NB;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrguchState[iInIterator][iOutIterator]=puchIn[iVectorIterator++]&0xff;
        }
    }
}
void StateToOut(unsigned char rgrguchState[4][NB], unsigned char *puchOut) {
    /**  @var iVectorIterator Integer to iterate through puchOut*/
    int iVectorIterator = 0;
    for(int iOutIterator = 0;iOutIterator<NB;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            puchOut[iVectorIterator++] = rgrguchState[iInIterator][iOutIterator];
        }
    }
}
/**Cipher Functions*/
void AddRoundKey(int iNr, int round, unsigned char rgrguchState[4][NB],
                 unsigned char rgrguchKeySchedule[NB * (iNr + 1)][4]) {
    for (int iOutIterator = 0; iOutIterator < NB; iOutIterator++) {
        for (int iInIterator = 0; iInIterator < 4; iInIterator++) {
            rgrguchState[iInIterator][iOutIterator] =
                rgrguchState[iInIterator][iOutIterator] ^
                rgrguchKeySchedule[round * NB + iOutIterator][iInIterator];
        }
    }
}
void SubState(unsigned char rgrguchState[4][NB]) {
    for (int iOutIterator = 0; iOutIterator < NB; iOutIterator++) {
        for (int iInIterator = 0; iInIterator < 4; iInIterator++) {
            rgrguchState[iInIterator][iOutIterator] =
                SubByte(rgrguchState[iInIterator][iOutIterator]);
        }
    }
}
void ShiftRows(unsigned char rgrguchState[4][NB]) {
    /**  @var puchTemp Pointer to integer array for storing the
      *       shifted version of the row*/
    unsigned char *puchTemp =
        (unsigned char *)malloc(NB * sizeof(unsigned char));
    for (int iOutIterator = 1; iOutIterator < 4; iOutIterator++) {
        for (int iInIterator0 = 0; iInIterator0 < NB; iInIterator0++) {
            puchTemp[(iInIterator0 - iOutIterator + NB) % NB] =
                rgrguchState[iOutIterator][iInIterator0];
        }
        for (int iInIterator1 = 0; iInIterator1 < NB; iInIterator1++) {
            rgrguchState[iOutIterator][iInIterator1] = puchTemp[iInIterator1];
        }
    }
    free(puchTemp);
}
void MixColumns(unsigned char rgrguchState[4][NB]){
    /**  @var rgrgiOldState 2-D Integer array for storing old State*/
    unsigned char rgrguchOldState[4][NB];
    for (int iOutIterator = 0; iOutIterator < NB; iOutIterator++) {
        for (int iInIterator = 0; iInIterator < 4; iInIterator++) {
            rgrguchOldState[iInIterator][iOutIterator] =
                rgrguchState[iInIterator][iOutIterator];
        }
    }
    for (int iIterator = 0; iIterator < NB; iIterator++) {
        rgrguchState[0][iIterator] = Mult(0x02, rgrguchOldState[0][iIterator]) ^
                                     Mult(0x03, rgrguchOldState[1][iIterator]) ^
                                     rgrguchOldState[2][iIterator] ^
                                     rgrguchOldState[3][iIterator];
        rgrguchState[1][iIterator] = rgrguchOldState[0][iIterator] ^
                                     Mult(0x02, rgrguchOldState[1][iIterator]) ^
                                     Mult(0x03, rgrguchOldState[2][iIterator]) ^
                                     rgrguchOldState[3][iIterator];
        rgrguchState[2][iIterator] = rgrguchOldState[0][iIterator] ^
                                     rgrguchOldState[1][iIterator] ^
                                     Mult(0x02, rgrguchOldState[2][iIterator]) ^
                                     Mult(0x03, rgrguchOldState[3][iIterator]);
        rgrguchState[3][iIterator] = Mult(0x03, rgrguchOldState[0][iIterator]) ^
                                     rgrguchOldState[1][iIterator] ^
                                     rgrguchOldState[2][iIterator] ^
                                     Mult(0x02, rgrguchOldState[3][iIterator]);
    }
}
void Cipher(int iNr, unsigned char *puchIn, unsigned char *puchOut,
            unsigned char rgrguchKeySchedule[NB * (iNr + 1)][4]) {
    if (iNr != 10 && iNr != 12 && iNr != 14) {
        return;
    }
    /**  @var rgrguchState 2-D Integer array for storing State*/
    unsigned char rgrguchState[4][NB];
    InToState(puchIn, rgrguchState);
    AddRoundKey(iNr, 0, rgrguchState, rgrguchKeySchedule);

    for (int round = 1; round < iNr; round++) {
        SubState(rgrguchState);
        ShiftRows(rgrguchState);
        MixColumns(rgrguchState);
        AddRoundKey(iNr, round, rgrguchState, rgrguchKeySchedule);
    }

    SubState(rgrguchState);
    ShiftRows(rgrguchState);
    AddRoundKey(iNr, iNr, rgrguchState, rgrguchKeySchedule);

    StateToOut(rgrguchState, puchOut);
}
/**Inverse Cipher Functions*/
void InvShiftRows(unsigned char rgrguchState[4][NB]) {
    /**  @var piTemp Pointer to integer array for storing the
      *       shifted version of the row*/
    unsigned char *puchTemp =
        (unsigned char *)malloc(NB * sizeof(unsigned char));
    for (int iOutIterator = 1; iOutIterator < 4; iOutIterator++) {
        for (int iInIterator0 = 0; iInIterator0 < NB; iInIterator0++) {
            puchTemp[(iInIterator0 + iOutIterator) % NB] =
                rgrguchState[iOutIterator][iInIterator0];
        }
        for (int iInIterator1 = 0; iInIterator1 < NB; iInIterator1++) {
            rgrguchState[iOutIterator][iInIterator1] = puchTemp[iInIterator1];
        }
    }
    free(puchTemp);
}
void InvSubBytes(unsigned char rgrguchState[4][NB]){
    /**
      *  An integer represented as 0xXY is substituted with
      *  InvSbox[X][Y]
      *  @var iX Integer for representing left nibble for use
      *       as described above
      *  @var uchY Integer for representing right nibble for use
      *       as described above
      */
    unsigned char uchX, uchY;
    /**  @var rgrguchInvSbox 2-D integer array as defined in FIPS 197*/
    unsigned char rgrguchInvSbox[16][16] = {
        {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
         0x81, 0xf3, 0xd7, 0xfb},
        {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44,
         0xc4, 0xde, 0xe9, 0xcb},
        {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b,
         0x42, 0xfa, 0xc3, 0x4e},
        {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
         0x6d, 0x8b, 0xd1, 0x25},
        {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
         0x5d, 0x65, 0xb6, 0x92},
        {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57,
         0xa7, 0x8d, 0x9d, 0x84},
        {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
         0xb8, 0xb3, 0x45, 0x06},
        {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03,
         0x01, 0x13, 0x8a, 0x6b},
        {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce,
         0xf0, 0xb4, 0xe6, 0x73},
        {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
         0x1c, 0x75, 0xdf, 0x6e},
        {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e,
         0xaa, 0x18, 0xbe, 0x1b},
        {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe,
         0x78, 0xcd, 0x5a, 0xf4},
        {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
         0x27, 0x80, 0xec, 0x5f},
        {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f,
         0x93, 0xc9, 0x9c, 0xef},
        {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c,
         0x83, 0x53, 0x99, 0x61},
        {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
         0x55, 0x21, 0x0c, 0x7d}};
    for (int iOutIterator = 0; iOutIterator < 4; iOutIterator++) {
        for (int iInIterator = 0; iInIterator < NB; iInIterator++) {
            uchY = rgrguchState[iOutIterator][iInIterator] & 0x0f;
            uchX = rgrguchState[iOutIterator][iInIterator] >> 4;
            rgrguchState[iOutIterator][iInIterator] =
                rgrguchInvSbox[uchX][uchY];
        }
    }
}
void InvMixColumns(unsigned char rgrguchState[4][NB]) {
    /**  @var rgrguchOldState 2-D Integer array for storing old State*/
    unsigned char rgrguchOldState[4][NB];
    for (int iOutIterator = 0; iOutIterator < NB; iOutIterator++) {
        for (int iInIterator = 0; iInIterator < 4; iInIterator++) {
            rgrguchOldState[iInIterator][iOutIterator] =
                rgrguchState[iInIterator][iOutIterator];
        }
    }
    for (int iIterator = 0; iIterator < NB; iIterator++) {
        rgrguchState[0][iIterator] = Mult(0x0e, rgrguchOldState[0][iIterator]) ^
                                     Mult(0x0b, rgrguchOldState[1][iIterator]) ^
                                     Mult(0x0d, rgrguchOldState[2][iIterator]) ^
                                     Mult(0x09, rgrguchOldState[3][iIterator]);
        rgrguchState[1][iIterator] = Mult(0x09, rgrguchOldState[0][iIterator]) ^
                                     Mult(0x0e, rgrguchOldState[1][iIterator]) ^
                                     Mult(0x0b, rgrguchOldState[2][iIterator]) ^
                                     Mult(0x0d, rgrguchOldState[3][iIterator]);
        rgrguchState[2][iIterator] = Mult(0x0d, rgrguchOldState[0][iIterator]) ^
                                     Mult(0x09, rgrguchOldState[1][iIterator]) ^
                                     Mult(0x0e, rgrguchOldState[2][iIterator]) ^
                                     Mult(0x0b, rgrguchOldState[3][iIterator]);
        rgrguchState[3][iIterator] = Mult(0x0b, rgrguchOldState[0][iIterator]) ^
                                     Mult(0x0d, rgrguchOldState[1][iIterator]) ^
                                     Mult(0x09, rgrguchOldState[2][iIterator]) ^
                                     Mult(0x0e, rgrguchOldState[3][iIterator]);
    }
}
void InvCipher(int iNr, unsigned char *puchIn, unsigned char *puchOut,
               unsigned char rgrguchKeySchedule[NB * (iNr + 1)][4]) {
    if (iNr != 10 && iNr != 12 && iNr != 14) {
        return;
    }
    /**  @var rgrgiState 2-D Integer array for storing State*/
    unsigned char rgrguchState[4][NB];
    InToState(puchIn, rgrguchState);

    AddRoundKey(iNr, iNr, rgrguchState, rgrguchKeySchedule);

    for (int round = iNr - 1; round > 0; round--) {
/**        printf("Start Round %d\n",iNr-round);
        PrintStateVector(NB,rgrguchState);*/
        InvShiftRows(rgrguchState);
/**        printf("After Shift Rows, Round %d\n",iNr-round);
        PrintStateVector(NB,rgrguchState);*/
        InvSubBytes(rgrguchState);
/**        printf("After Sub Bytes, Round %d\n",iNr-round);
        PrintStateVector(NB,rgrguchState);*/

        AddRoundKey(iNr, round, rgrguchState, rgrguchKeySchedule);
/**        printf("After Add Round, Round %d\n",iNr-round);
        PrintStateVector(NB,rgrguchState);*/
        InvMixColumns(rgrguchState);
    }

    InvShiftRows(rgrguchState);
/**    printf("After Shift Rows, Round %d\n",iNr);
    PrintStateVector(NB,rgrguchState);*/
    InvSubBytes(rgrguchState);
/**    printf("After Sub Bytes, Round %d\n",iNr);
    PrintStateVector(NB,rgrguchState);*/
    AddRoundKey(iNr, 0, rgrguchState, rgrguchKeySchedule);

    StateToOut(rgrguchState, puchOut);
}
