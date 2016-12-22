#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
void RotArr(int iSize,int rgiW[iSize]);
int SubByte(int iByte);
void SubWord(int rgiW[4]);
void KeyExpansion(int iNb,int iNr, int iNk,int rgiKey[iNk*4], int rgrgiKeySchedule[iNb*(iNr+1)][4]);
void InToState(int iNb,int * piIn,int rgrgiState[4][iNb]);
void StateToOut(int iNb,int rgrgiState[4][iNb],int * piOut);
void AddRoundKey(int iNr,int iNb,int round,int rgrgiState[4][iNb],int rgrgiKeySchedule[iNb*(iNr+1)][4]);
void SubState(int iNb,int rgrgiState[4][iNb]);
void ShiftRows(int iNb,int rgrgiState[4][iNb]);
void MixColumns(int iNb,int rgrgiState[4][iNb]);
void Reverse(int * piArr,int iSize);
int Mult(int iX,int iY);
void Cipher(int iNb,int iNr,int * piIn,int * piOut,int rgrgiKeySchedule[iNb*(iNr+1)][4]);
void InvShiftRows(int iNb,int rgrgiState[4][iNb]);
void InvSubBytes(int iNb,int rgrgiState[4][iNb]);
void InvMixColumns(int iNb,int rgrgiState[4][iNb]);
void InvCipher(int iNb,int iNr,int * piIn,int * piOut,int rgrgiKeySchedule[iNb*(iNr+1)][4]);
void PrintData(int iSize,int iOffset,int * piData);
int CharToBytes(int iSize,char * pchArr,int * piBytes);
int BytesToChar(int iSize,int * piBytes,char * pchArr,int iOffset);
void FillIn(int iSize,int iOffset,int rgiIn[iSize],int * piData);
void XorVector(int iSize,int rgiVectorPlain[iSize],int rgiVector[iSize]);
int *EncCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piData);
int *Enc(int iMode,int iKeySize,int iDataSize, int rgiIv[16],int rgiKey[iKeySize],int * piData);
int *DecCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData);
int *Dec(int iMode,int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData);