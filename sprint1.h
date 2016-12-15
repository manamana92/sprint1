#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
void rotArr(int size,int w[size]);
int subByte(int byte);
void subWord(int w[4]);
void keyExpansion(int nb,int nr,int nk,int key[nk*4],int w[nb*(nr+1)][4]);
void inToState(int nb,int in[4*nb],int state[4][nb]);
void stateToOut(int nb,int state[4][nb],int out[4*nb]);
void addRoundKey(int nr,int nb,int round,int state[4][nb],int key[nb*(nr+1)][4]);
void subState(int nb,int state[4][nb]);
void shiftRows(int nb,int state[4][nb]);
void mixColumns(int nb,int state[4][nb]);
void reverse(int * arr,int size);
int ericMult(int x,int y);
void cipher(int nb,int nr,int in[4*nb],int out[4*nb],int w[nb*(nr+1)][4]);
void invShiftRows(int nb,int state[4][nb]);
void invSubBytes(int nb,int state[4][nb]);
void invMixColumns(int nb,int state[4][nb]);
void invCipher(int nb,int nr,int * in,int * out,int w[nb*(nr+1)][4]);
int charToBytes(int size,char * arr,int * bytes);
void fillIn(int size,int offset,int in[size],int * data);
void xorVector(int size,int vectorPlain[size],int vector[size]);
int encCBC(int size,int dataSize,int iv[size],int key[size],int * data,int * encData);
int enc(int mode,int size,int dataSize,int iv[size],int key[size],int * data,int * encData);
