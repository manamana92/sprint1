#include "sprint1.h"
void printState(int nb,int state[4][nb]){
    for(int i = 0;i<4;i++){
        for(int j = 0;j<nb;j++){
            printf("%02x ",state[i][j]);
        }
        printf("\n");
    }
}
void printDataCi(int size,int *data){
    for(int i = 0;i<size;i++){
        printf("%02x ",data[i]);
    }
    printf("\n");
}
void printStateVector(int nb,int state[4][nb]){
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            printf("%02x ",state[j][i]);
        }
    }
    printf("\n");
}
void reverse(int * arr,int size){
    int buff = 0;
    for(int i = 0;i<(size/2);i++){
        buff = arr[i];
        arr[i]=arr[size-i-1];
        arr[size-i-1]=buff;
    }
}
int ericMult(int x,int y){
    int arrSize = 1;
    /**printf("x size: %d y size: %d",sizeof(x),sizeof(y));*/
    int *bits = (int *)malloc(sizeof(int));
    bits[0]=x&1;
    x=x>>1;
    int count = 1;
    while(x>0){
	bits = (int *)realloc(bits,++arrSize*sizeof(int));
        bits[count]=x&1;
        x=x>>1;
	count++;
    }
    reverse(bits,arrSize);
    int retVal = 0;
    for(int i=0;i<arrSize;i++){
        if(retVal){
             retVal*=2;
             if(retVal&0x100){
                 retVal = retVal^0x011b;
             }
        }
        if(bits[i]){
            retVal = retVal^y;
        }
    }
    free(bits);
    return retVal;
}
int mult(int x,int y){

    int ret = x*y;
    printf("%x * %x = %x or ",x,y,ret);
    if(ret>0x00ff){
        ret = ret ^ 0x011b;
    }
    printf("%x\n",ret);
    return ret;
}
void inToState(int nb,int * in,int state[4][nb]){
    int inIterator = 0;
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            state[j][i]=in[inIterator++];
        }
    }
}
void stateToOut(int nb,int state[4][nb],int * out){
    int outIterator = 0;
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            out[outIterator++] = state[j][i];
        }
    }
}
/**Cipher Functions*/
void addRoundKey(int nr,int nb,int round,int state[4][nb],int key[nb*(nr+1)][4]){
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            state[j][i]=state[j][i]^key[round*nb+i][j];
        }
    }
}
void subState(int nb,int state[4][nb]){
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            state[j][i] = subByte(state[j][i]);
        }
    }
}
void shiftRows(int nb,int state[4][nb]){
    for(int j = 1;j<4;j++){
        for(int k = 0;k<j;k++){
            rotArr(nb,state[j]);
        }
    }
}
void mixColumns(int nb,int state[4][nb]){
    int oldState[4][nb];
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            oldState[j][i]=state[j][i];
        }
    }
    for(int i = 0;i<nb;i++){
        state[0][i]=ericMult(0x02,oldState[0][i])^ericMult(0x03,oldState[1][i])^oldState[2][i]^oldState[3][i];
        state[1][i]=oldState[0][i]^ericMult(0x02,oldState[1][i])^ericMult(0x03,oldState[2][i])^oldState[3][i];
        state[2][i]=oldState[0][i]^oldState[1][i]^ericMult(0x02,oldState[2][i])^ericMult(0x03,oldState[3][i]);
        state[3][i]=ericMult(0x03,oldState[0][i])^oldState[1][i]^oldState[2][i]^ericMult(0x02,oldState[3][i]);
    }
}
void cipher(int nb,int nr,int * in,int * out,int w[nb*(nr+1)][4]){
    int state[4][nb];
    inToState(nb,in,state);
//    printState(nb,state);
    addRoundKey(nr,nb,0,state,w);
//    printf("After Round Key Addition");
//    printState(nb,state);

    for(int round = 1;round<nr;round++){
        subState(nb,state);
	shiftRows(nb,state);
	mixColumns(nb,state);
	addRoundKey(nr,nb,round,state,w);

	
    }

    subState(nb,state);
    shiftRows(nb,state);
    addRoundKey(nr,nb,nr,state,w);


    stateToOut(nb,state,out);
}
/**Inverse Cipher Functions*/
void invShiftRows(int nb,int state[4][nb]){
    int *temp=(int *)malloc(nb*sizeof(int));
    for(int j = 1;j<4;j++){
        for(int k = 0;k<nb;k++){
            temp[(k+j)%nb]=state[j][k];
	}
	for(int l = 0;l<nb;l++){
            state[j][l]=temp[l];
        }
    }
    free(temp);
}
void invSubBytes(int nb,int state[4][nb]){
    int x,y;
    int invSbox[16][16]={{0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},{0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},{0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},{0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},{0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},{0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},{0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},{0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},{0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},{0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},{0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},{0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},{0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},{0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},{0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},{0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}};
    for(int i = 0;i<4;i++){
        for(int j = 0;j<nb;j++){
            y=state[i][j]&0x0f;
	    x=state[i][j]>>4;
	    state[i][j]=invSbox[x][y];
        }
    }
}
void invMixColumns(int nb,int state[4][nb]){
    int oldState[4][nb];
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            oldState[j][i]=state[j][i];
        }
    }
    for(int i =0;i<nb;i++){
        state[0][i]=ericMult(0x0e,oldState[0][i])^ericMult(0x0b,oldState[1][i])^ericMult(0x0d,oldState[2][i])^ericMult(0x09,oldState[3][i]);
	state[1][i]=ericMult(0x09,oldState[0][i])^ericMult(0x0e,oldState[1][i])^ericMult(0x0b,oldState[2][i])^ericMult(0x0d,oldState[3][i]);
        state[2][i]=ericMult(0x0d,oldState[0][i])^ericMult(0x09,oldState[1][i])^ericMult(0x0e,oldState[2][i])^ericMult(0x0b,oldState[3][i]);
	state[3][i]=ericMult(0x0b,oldState[0][i])^ericMult(0x0d,oldState[1][i])^ericMult(0x09,oldState[2][i])^ericMult(0x0e,oldState[3][i]);
    }
}
void invCipher(int nb,int nr,int * in,int * out,int w[nb*(nr+1)][4]){
    int state[4][nb];
    inToState(nb,in,state);

    addRoundKey(nr,nb,nr,state,w);

    for(int round = nr-1;round>0;round--){
//        printf("Start Round %d\n",nr-round);
//	printStateVector(nb,state);
        invShiftRows(nb,state);
//	printf("After Shift Rows, Round %d\n",nr-round);
//	printStateVector(nb,state);
	invSubBytes(nb,state);
//        printf("After Sub Bytes, Round %d\n",nr-round);
//	printStateVector(nb,state);

	addRoundKey(nr,nb,round,state,w);
//	printf("After Add Round, Round %d\n",nr-round);
//	printStateVector(nb,state);
	invMixColumns(nb,state);
    }

    invShiftRows(nb,state);
//    printf("After Shift Rows, Round %d\n",nr);
//    printStateVector(nb,state);
    invSubBytes(nb,state);
//    printf("After Sub Bytes, Round %d\n",nr);
//    printStateVector(nb,state);
    addRoundKey(nr,nb,0,state,w);

    stateToOut(nb,state,out);
}
/**
int main(void){
    int mainNb = 4;
    int in[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
    int key[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    int state[4][mainNb];
    inToState(mainNb,in,state);
    for(int i = 0;i<4;i++){
        for(int j = 0;j<mainNb;j++){
            printf("%02x ",state[i][j]);
        }
        printf("\n");
    }

    mixColumns(mainNb,state);
    
    for(int i = 0;i<4;i++){
        for(int j = 0;j<mainNb;j++){
            printf("%02x ",state[i][j]);
        }
        printf("\n");
    }

    int w[44][4]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
    keyExpansion(4,10,4,key,w);
    int out[16];

    cipher(mainNb,10,in,out,w);


    for(int j = 0;j<16;j++){
        in[j]=0;
    }
    printf("Out\n");
    printDataCi(16,out);
    printf("In\n");
    printDataCi(16,in);
    invCipher(mainNb,10,out,in,w);

    printf("After inverse\n");
    printDataCi(16,in);

    return 0;
}*/
