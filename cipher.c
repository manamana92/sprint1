#include "sprint1.h"
void printState(int nb,int state[4][nb]){
    for(int i = 0;i<4;i++){
        for(int j = 0;j<nb;j++){
            printf("%02x ",state[i][j]);
        }
        printf("\n");
    }
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
void inToState(int nb,int in[4*nb],int state[4][nb]){
    int inIterator = 0;
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            state[j][i]=in[inIterator++];
        }
    }
}
void stateToOut(int nb,int state[4][nb],int out[4*nb]){
    int outIterator = 0;
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            out[outIterator++] = state[j][i];
        }
    }
}
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
void cipher(int nb,int nr,int in[4*nb],int out[4*nb],int w[nb*(nr+1)][4]){
    int state[4][nb];
    inToState(nb,in,state);
    printState(nb,state);
    addRoundKey(nr,nb,0,state,w);
    printf("After Round Key Addition");
    printState(nb,state);

    for(int round = 1;round<nr;round++){
        subState(nb,state);
	shiftRows(nb,state);
	mixColumns(nb,state);
	addRoundKey(nr,nb,round,state,w);

	
    }

    subState(nb,state);
    shiftRows(nb,state);
    addRoundKey(nr,nb,nr,state,w);

    for(int i = 0;i<4;i++){
        for(int j = 0;j<nb;j++){
            printf("%02x ",state[i][j]);
        }
        printf("\n");
    }

    stateToOut(nb,state,out);
}
/**int main(void){
    int mainNb = 4;
    int key[16]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    int in[16]={0x32,0x43,0xf6,0xa8,0x88,0x5a,0x30,0x8d,0x31,0x31,0x98,0xa2,0xe0,0x37,0x07,0x34};
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

    for(int i=0;i<44;i++){
        printf("Round %d\t",i/4);
	for(int j = 0;j<4;j++){
            printf("%02x",w[i][j]);
	}
	printf("\n");
    }


    return 0;
}*/
