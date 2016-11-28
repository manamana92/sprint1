#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
void rotArr(int size,int w[size]);
void subByte(int byte);
void subWord(int w[4]);
void keyExpansion(int nb,int nr,int nk, int key[nk*4],int w[nb*(nr+1)][4]);
void inToState(int nb,int in[4*nb],int state[4][nb]);
void stateToOut(int nb,int state[4][nb],int out[4*nb]);
void addRoundKey(int nr,int nb,int round, int state[4][nb],int key[nb*(nr+1)][4]);

