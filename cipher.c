#include "sprint1.h"
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
            state[j][i]=state[j][i]^key[round*nb][j];
        }
    }
}
void subState(int nb,int state[4][nb]){
    for(int i = 0;i<nb;i++){
        for(int j = 0;j<4;j++){
            subByte(state[j][i]);
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
        state[0][i]=mult(0x02,oldState[0][i])^mult(0x03,oldState[1][i])^oldState[2][i]^oldState[3][i]);
        state[1][i]=oldState[0][i]^mult(0x02,oldState[1][i])^mult(0x03,oldState[2][i])^oldState[3][i]);
        state[2][i]=oldState[0][i]^oldState[1][i]^mult(0x02,oldState[2][i])^mult(0x03,oldState[3][i]);
        state[3][i]=mult(0x03,oldState[0][i])^oldState[1][i]^oldState[2][i]^mult(0x02,oldState[3][i]);
    }
}
/**void Cipher(int nb,int nr,int in[4*nb],int out[4*nb],int w[nb*(nr+1)][4]){
}*/
int main(void){
    int mainNb = 4;
    int in[16]={0xd4,0xbf,0x5d,0x30,0xe0,0xb4,0x52,0xae,0xb8,0x41,0x11,0xf1,0x1e,0x27,0x98,0xe5};
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

    return 0;
}
