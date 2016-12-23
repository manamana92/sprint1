#include "sprint1.h"
void PrintState(int iNb,int rgrgiState[4][iNb]){
    for(int iOutIterator = 0;iOutIterator<4;iOutIterator++){
        for(int iInIterator = 0;iInIterator<iNb;iInIterator++){
            printf("%02x ",rgrgiState[iOutIterator][iInIterator]);
        }
        printf("\n");
    }
}
void PrintDataCi(int iSize,int *piData){
    for(int iIterator = 0;iIterator<iSize;iIterator++){
        printf("%02x ",piData[iIterator]);
    }
    printf("\n");
}
void PrintStateVector(int iNb,int rgrgiState[4][iNb]){
    for(int iIterator = 0;iIterator<iNb;iIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            printf("%02x ",rgrgiState[iInIterator][iIterator]);
        }
    }
    printf("\n");
}
void Reverse(int * piArr,int iSize){
    int iBuff = 0;
    for(int iIterator = 0;iIterator<(iSize/2);iIterator++){
        iBuff = piArr[iIterator];
        piArr[iIterator]=piArr[iSize-iIterator-1];
        piArr[iSize-iIterator-1]=iBuff;
    }
}
int Mult(int iX,int iY){
    int iArrSize = 1;
    int *piBits = (int *)malloc(sizeof(int));
    piBits[0]=iX&1;
    iX=iX>>1;
    int iCount = 1;
    while(iX>0){
        piBits = (int *)realloc(piBits,++iArrSize*sizeof(int));
        piBits[iCount]=iX&1;
        iX=iX>>1;
        iCount++;
    }
    Reverse(piBits,iArrSize);
    int iRetVal = 0;
    for(int iIterator=0;iIterator<iArrSize;iIterator++){
        if(iRetVal){
             iRetVal*=2;
             if(iRetVal&0x100){
                 iRetVal = iRetVal^0x011b;
             }
        }
        if(piBits[iIterator]){
            iRetVal = iRetVal^iY;
        }
    }
    free(piBits);
    return iRetVal;
}

void InToState(int iNb,int * piIn,int rgrgiState[4][iNb]){
    int iVectorIterator = 0;
    for(int iOutIterator = 0;iOutIterator<iNb;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrgiState[iInIterator][iOutIterator]=piIn[iVectorIterator++];
        }
    }
}
void StateToOut(int iNb,int rgrgiState[4][iNb],int * piOut){
    int iVectorIterator = 0;
    for(int iOutIterator = 0;iOutIterator<iNb;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            piOut[iVectorIterator++] = rgrgiState[iInIterator][iOutIterator];
        }
    }
}
/**Cipher Functions*/
void AddRoundKey(int iNr,int iNb,int round,int rgrgiState[4][iNb],int rgrgiKeySchedule[iNb*(iNr+1)][4]){
    for(int i = 0;i<iNb;i++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrgiState[iInIterator][i]=rgrgiState[iInIterator][i]^rgrgiKeySchedule[round*iNb+i][iInIterator];
        }
    }
}
void SubState(int iNb,int rgrgiState[4][iNb]){
    for(int iOutIterator = 0;iOutIterator<iNb;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrgiState[iInIterator][iOutIterator] = SubByte(rgrgiState[iInIterator][iOutIterator]);
        }
    }
}
void ShiftRows(int iNb,int rgrgiState[4][iNb]){
    for(int iOutIterator = 1;iOutIterator<4;iOutIterator++){
        for(int iInIterator = 0;iInIterator<iOutIterator;iInIterator++){
            RotArr(iNb,rgrgiState[iOutIterator]);
        }
    }
}
void MixColumns(int iNb,int rgrgiState[4][iNb]){
    int rgrgiOldState[4][iNb];
    for(int iOutIterator = 0;iOutIterator<iNb;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrgiOldState[iInIterator][iOutIterator]=rgrgiState[iInIterator][iOutIterator];
        }
    }
    for(int iIterator = 0;iIterator<iNb;iIterator++){
        rgrgiState[0][iIterator]=Mult(0x02,rgrgiOldState[0][iIterator])^Mult(0x03,rgrgiOldState[1][iIterator])^rgrgiOldState[2][iIterator]^rgrgiOldState[3][iIterator];
        rgrgiState[1][iIterator]=rgrgiOldState[0][iIterator]^Mult(0x02,rgrgiOldState[1][iIterator])^Mult(0x03,rgrgiOldState[2][iIterator])^rgrgiOldState[3][iIterator];
        rgrgiState[2][iIterator]=rgrgiOldState[0][iIterator]^rgrgiOldState[1][iIterator]^Mult(0x02,rgrgiOldState[2][iIterator])^Mult(0x03,rgrgiOldState[3][iIterator]);
        rgrgiState[3][iIterator]=Mult(0x03,rgrgiOldState[0][iIterator])^rgrgiOldState[1][iIterator]^rgrgiOldState[2][iIterator]^Mult(0x02,rgrgiOldState[3][iIterator]);
    }
}
void Cipher(int iNb,int iNr,int * piIn,int * piOut,int rgrgiKeySchedule[iNb*(iNr+1)][4]){
    int rgrgiState[4][iNb];
    InToState(iNb,piIn,rgrgiState);
    AddRoundKey(iNr,iNb,0,rgrgiState,rgrgiKeySchedule);

    for(int round = 1;round<iNr;round++){
        SubState(iNb,rgrgiState);
        ShiftRows(iNb,rgrgiState);
        MixColumns(iNb,rgrgiState);
        AddRoundKey(iNr,iNb,round,rgrgiState,rgrgiKeySchedule);
    }

    SubState(iNb,rgrgiState);
    ShiftRows(iNb,rgrgiState);
    AddRoundKey(iNr,iNb,iNr,rgrgiState,rgrgiKeySchedule);


    StateToOut(iNb,rgrgiState,piOut);
}
/**Inverse Cipher Functions*/
void InvShiftRows(int iNb,int rgrgiState[4][iNb]){
    int *piTemp=(int *)malloc(iNb*sizeof(int));
    for(int iOutIterator = 1;iOutIterator<4;iOutIterator++){
        for(int iInIterator0 = 0;iInIterator0<iNb;iInIterator0++){
            piTemp[(iInIterator0+iOutIterator)%iNb]=rgrgiState[iOutIterator][iInIterator0];
	}
	    for(int iInIterator1 = 0;iInIterator1<iNb;iInIterator1++){
            rgrgiState[iOutIterator][iInIterator1]=piTemp[iInIterator1];
        }
    }
    free(piTemp);
}
void InvSubBytes(int iNb,int rgrgiState[4][iNb]){
    int iX,iY;
    int rgrgiInvSbox[16][16]={{0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},{0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},{0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},{0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},{0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},{0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},{0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},{0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},{0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},{0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},{0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},{0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},{0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},{0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},{0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},{0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}};
    for(int iOutIterator = 0;iOutIterator<4;iOutIterator++){
        for(int iInIterator = 0;iInIterator<iNb;iInIterator++){
            iY=rgrgiState[iOutIterator][iInIterator]&0x0f;
            iX=rgrgiState[iOutIterator][iInIterator]>>4;
            rgrgiState[iOutIterator][iInIterator]=rgrgiInvSbox[iX][iY];
        }
    }
}
void InvMixColumns(int iNb,int rgrgiState[4][iNb]){
    int rgrgiOldState[4][iNb];
    for(int iOutIterator = 0;iOutIterator<iNb;iOutIterator++){
        for(int iInIterator = 0;iInIterator<4;iInIterator++){
            rgrgiOldState[iInIterator][iOutIterator]=rgrgiState[iInIterator][iOutIterator];
        }
    }
    for(int iIterator =0;iIterator<iNb;iIterator++){
        rgrgiState[0][iIterator]=Mult(0x0e,rgrgiOldState[0][iIterator])^Mult(0x0b,rgrgiOldState[1][iIterator])^Mult(0x0d,rgrgiOldState[2][iIterator])^Mult(0x09,rgrgiOldState[3][iIterator]);
        rgrgiState[1][iIterator]=Mult(0x09,rgrgiOldState[0][iIterator])^Mult(0x0e,rgrgiOldState[1][iIterator])^Mult(0x0b,rgrgiOldState[2][iIterator])^Mult(0x0d,rgrgiOldState[3][iIterator]);
        rgrgiState[2][iIterator]=Mult(0x0d,rgrgiOldState[0][iIterator])^Mult(0x09,rgrgiOldState[1][iIterator])^Mult(0x0e,rgrgiOldState[2][iIterator])^Mult(0x0b,rgrgiOldState[3][iIterator]);
        rgrgiState[3][iIterator]=Mult(0x0b,rgrgiOldState[0][iIterator])^Mult(0x0d,rgrgiOldState[1][iIterator])^Mult(0x09,rgrgiOldState[2][iIterator])^Mult(0x0e,rgrgiOldState[3][iIterator]);
    }
}
void InvCipher(int iNb,int iNr,int * piIn,int * piOut,int rgrgiKeySchedule[iNb*(iNr+1)][4]){
    int rgrgiState[4][iNb];
    InToState(iNb,piIn,rgrgiState);

    AddRoundKey(iNr,iNb,iNr,rgrgiState,rgrgiKeySchedule);

    for(int round = iNr-1;round>0;round--){
/**        printf("Start Round %d\n",iNr-round);
        PrintStateVector(iNb,rgrgiState);*/
        InvShiftRows(iNb,rgrgiState);
/**        printf("After Shift Rows, Round %d\n",iNr-round);
        PrintStateVector(iNb,rgrgiState);*/
        InvSubBytes(iNb,rgrgiState);
/**        printf("After Sub Bytes, Round %d\n",iNr-round);
        PrintStateVector(iNb,rgrgiState);*/

        AddRoundKey(iNr,iNb,round,rgrgiState,rgrgiKeySchedule);
/**        printf("After Add Round, Round %d\n",iNr-round);
        PrintStateVector(iNb,rgrgiState);*/
        InvMixColumns(iNb,rgrgiState);
    }

    InvShiftRows(iNb,rgrgiState);
/**    printf("After Shift Rows, Round %d\n",iNr);
    PrintStateVector(iNb,rgrgiState);*/
    InvSubBytes(iNb,rgrgiState);
/**    printf("After Sub Bytes, Round %d\n",iNr);
    PrintStateVector(iNb,rgrgiState);*/
    AddRoundKey(iNr,iNb,0,rgrgiState,rgrgiKeySchedule);

    StateToOut(iNb,rgrgiState,piOut);
}

/**int main(void){
    int mainNb = 4;
    int in[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    int key128[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    int key192[24]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17};
    int rgrgiState[4][mainNb];
    InToState(mainNb,in,rgrgiState);
    for(int i = 0;i<4;i++){
        for(int iInIterator = 0;iInIterator<mainNb;iInIterator++){
            printf("%02x ",rgrgiState[i][iInIterator]);
        }
        printf("\n");
    }

    MixColumns(mainNb,rgrgiState);
    
    for(int i = 0;i<4;i++){
        for(int iInIterator = 0;iInIterator<mainNb;iInIterator++){
            printf("%02x ",rgrgiState[i][iInIterator]);
        }
        printf("\n");
    }

    int rgrgiKeySchedule[52][4];={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    keyExpansion(4,12,6,key192,rgrgiKeySchedule);
    int pOut[16];

    Cipher(mainNb,12,in,pOut,rgrgiKeySchedule);


    for(int j = 0;j<16;j++){
        in[j]=0;
    }
    printf("Out\n");
    PrintDataCi(16,pOut);
    printf("In\n");
    PrintDataCi(16,in);
    InvCipher(mainNb,12,pOut,in,rgrgiKeySchedule);

    printf("After inverse\n");
    PrintDataCi(16,in);

    return 0;
}*/
