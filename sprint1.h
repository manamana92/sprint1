/**
  * @file sprint1.h
  * @brief includes and function prototypes
  * 
  * This contains the function prototypes for
  * the sprintf program.
  *
  * @author Christopher Orsini
  *
  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
/**
  *  @brief Rotates the integer range rgiW by
  *         one.
  * 
  *  @param iSize the size of the range rgiW
  *  @param rgiW the range to rotate by one
  */
void RotArr(int iSize,int rgiW[iSize]);
/**
  *  @brief Performs a byte substitution in Sbox
  *         as defined in FIPS 197
  *  
  *  This accomplishes this by taking an integer
  *  0xXY and return Sbox[X][Y]
  *
  *  @param iByte the byte to substitute through
  *         the Sbox
  *  @return The result of the substitute of iByte
  *          through the Sbox
  */
int SubByte(int iByte);
/**
  *  @brief Performs a SubByte() operation for each
  *         byte in a word rgiW
  *
  *  The accomplishes this through a simple loop
  *  through rgiW. Then call SubByte and store
  *  returned value in rgiW.
  *
  *  @param rgiW the input word to run through
  *         byte substitution.
  */
void SubWord(int rgiW[4]);
/**
  *  @brief Runs the Key Expansion defined in
  *         FIPS 197
  *  
  *  @param iNb The number of columns comprising the
  *         State. This is defined by the size of
  *         the key.
  *  @param iNr The number of rounds. This is
  *         determined by the size of the key.
  *  @param iNk the number of words in the key.
  *  @param rgiKey The integer array containing the
  *	        key
  *  @param rgrgiKeySchedule The 2 dimensional
  *         integer array used to store the result
  *         from the Key Expansion routine. This
  *         is of size iNb*(iNr+1) words.
  *
  *  The Key Expansion routine stores the key in
  *  first iNk words. It then stores the previous
  *  word in integer array rgiTemp. If the round
  *  is a multiple of iNk, it will do the following:
  *      It then runs rgiTemp through RotArr
  *      then SubWord
  *      It then computes the first value of the
  *      Rcon and stores the value in iRcon
  *      It then xor the value with rgiTemp[0].
  *  Else if the key is 256-bit and round modulo
  *  iNk is 4, it will do the following:
  *      It will run rgiTemp through SubWord

  *  Next it stores rgrgiKeySchedule[round-iNk] xor
  *  rgiTemp in rgrgiKeySchedule[round].
  */
void KeyExpansion(int iNb,int iNr, int iNk,int rgiKey[iNk*4], int rgrgiKeySchedule[iNb*(iNr+1)][4]);
/**
  *  @brief Stores a vector in a 2-D integer array
  *
  *  @param iNb The number of columns comprising the
  *         State. This is defined by the size of
  *         the key.
  *  @param piIn The vector to store in the 2-D array
  *  @param rgrgiState The 2-D integer array to store
  *         the vector in.
  */
void InToState(int iNb,int * piIn,int rgrgiState[4][iNb]);
/**
  *  @brief Stores a 2-D integer array in a vector
  *
  *  @param iNb The number of columns comprising the
  *         State. This is defined by the size of
  *         the key.
  *  @param rgrgiState The 2-D integer array containing
  *         the State.
  *  @param piOut The vector to store the 2-D array in
  */
void StateToOut(int iNb,int rgrgiState[4][iNb],int * piOut);
/**
  *  @brief Add the round key from Key Schedule to State
  *
  *  @param iNb The number of columns comprising the
  *         State. This is defined by the size of
  *         the key.
  */
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