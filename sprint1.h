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
#define BLOCKSIZE 16
#define NB 4
/**
  *  @brief Rotates the integer range rgiW by
  *         one.
  * 
  *  @param rgiW the range to rotate by one
  */
void RotWord(unsigned char rguchW[4]);
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
unsigned char SubByte(unsigned char uchByte);
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
void SubWord(unsigned char rguchW[4]);
/**
  *  @brief Runs the Key Expansion defined in
  *         FIPS 197
  *  
  *  @param iNr The number of rounds. This is
  *         determined by the size of the key.
  *  @param iNk the number of words in the key.
  *  @param rgiKey The integer array containing the
  *	        key
  *  @param rgrgiKeySchedule The 2 dimensional
  *         integer array used to store the result
  *         from the Key Expansion routine. This
  *         is of size NB*(iNr+1) words.
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
void KeyExpansion(int iNr, int iNk,unsigned char rguchKey[iNk*4], unsigned char rgrguchKeySchedule[NB*(iNr+1)][4]);
/**
  *  @brief Stores a vector in a 2-D integer array
  *
  *  @param piIn The vector to store in the 2-D array
  *  @param rgrgiState The 2-D integer array to store
  *         the vector in.
  */
void InToState(unsigned char *puchIn,unsigned char rgrguchState[4][NB]);
/**
  *  @brief Stores a 2-D integer array in a vector
  *
  *  @param rgrgiState The 2-D integer array containing
  *         the State.
  *  @param piOut The vector to store the 2-D array in
  */
void StateToOut(unsigned char rgrguchState[4][NB],unsigned char *puchOut);
/**
  *  @brief Add the round key from Key Schedule to State
  *
  *  @param iNr The number of rounds. This is
  *         determined by the size of the key.
  *  @param round The current round of the cipher or inverse cipher
  *  @param rgrgiState A 2-D integer array of the State
  *  @param rgrgiKeySchedule The 2-D integer array of the Key Schedule
  *
  *  This is accomplished through 2 for loops. Within the
  *  for loop it stores the xor operation of the State and the
  *  key schedule at offset round*NB.
  */
void AddRoundKey(int iNr,int round,unsigned char rgrguchState[4][NB],unsigned char rgrguchKeySchedule[NB*(iNr+1)][4]);
/**
  *  @brief Performs a SubByte operation for each value in the state.
  *  
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is accomplished through 2 for loops. Within
  *  the for loop, the SubByte operation is performed, and
  *  the result is stored back in the rgrgiState
  */
void SubState(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Performs a row shift of a 2-D integer array
  *
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is accomplished through 2 for loops. The RotArr
  *  operation is performed the number of times based on
  *  the row number
  */
void ShiftRows(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Performs a manipulation of each column
  *  
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is performed by first copying the State to 2-D
  *  integer array rgrgiOldState. Then for each column,
  *  it performs manipulation by multiplying it with the
  *  matrix below:
  *   -         -
  *  |02 03 01 01|
  *  |01 02 03 01|
  *  |01 01 02 03|
  *  |03 01 01 02|
  *  The result of the manipulation is stored back in
  *  rgrgiState
  */
void MixColumns(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Simple function to revers the order of
  *         an integer array
  *
  *  @param piArr Pointer to an integer array
  *  @param iSize size of the integer array piArr
  *         points to
  *
  *  This is accomplished by simply using a for loop
  *  to go through half of the array using an iterator
  *  and swap each value with the value at iSize-iterator.
  */
void Reverse(unsigned char *puchiArr,int iSize);
/**
  *  @brief Performs a multiplication in the finite field
  *
  *  @param iX Integer to multiply
  *  @param iY Integer with which to multiply
  *
  *  This is accomplished by first splitting iX into an
  *  array representing the bit representation of iX. It
  *  then loops throught this array and performs the
  *  multiplication operation and returns the value of
  *  the product.
  */
int Mult(unsigned char uchX,unsigned char uchY);
/**
  *  @brief Function to perform AES encryption
  *
  *  @param iNr The number of rounds. This is
  *         determined by the size of the key.
  *  @param piIn The pointer to an integer array
  *         that contains the input plaintext to
  *         be encrypted.
  *
  *  @param piOut The pointer to an integer array
  *         to store the ciphertext.
  *  @param rgrgiKeySchedule The Key Schedule returned
  *         from the KeyExpansion function.
  *
  *  The Cipher operation is performed as follows:
  *  1. Copy contents from piIn to a 2-D integer array
  *     to represent the State known as rgrgiState
  *  2. Perform AddRoundKey operation on round 0
  *  3. For each round up to but not including iNr
  *     perform the following:
  *         SubState, ShiftRows, and MixColumns
  *         operations on rgrgiState
  *         Then perform AddRoundKey operation
  *         on specific round
  *  4. Perform SubState and ShiftRows on rgrgiState
  *  5. Perform AddRoundKey operation on round iNr.
  *  6. Copy contetnst from rgrgiState to piOut.
  */
void Cipher(int iNr,unsigned char *puchIn,unsigned char *puchOut,unsigned char rgrguchKeySchedule[NB*(iNr+1)][4]);
/**
  *  @brief Perform row shift in opposite direction
  *         of ShiftRows operation.
  *
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is acomplished by going through each row
  *  storing the values offset by the row number in
  *  an integer array pointed to by piTemp. These
  *  values are then stored back into rgrgiState.
  */
void InvShiftRows(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Performs a byte substitution based on
  *         InvSbox defined in FIPS 197
  *  
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is accomplished through 2 for loops. Within
  *  the for loops, each byte represented as 0xXY is
  *  substituted with InvSbox[X][Y]
  */  
void InvSubBytes(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Performs a manipulation of each column
  *  
  *  @param rgrgiState A 2-D integer array of the State
  *
  *  This is performed by first copying the State to 2-D
  *  integer array rgrgiOldState. Then for each column,
  *  it performs manipulation by multiplying it with the
  *  matrix below:
  *   -         -
  *  |0e 0b 0d 09|
  *  |09 0e 0b 0d|
  *  |0d 09 0e 0b|
  *  |0b 0d 09 0e|
  *  The result of the manipulation is stored back in
  *  rgrgiState
  */
void InvMixColumns(unsigned char rgrguchState[4][NB]);
/**
  *  @brief Function to perform AES decryption
  *
  *  @param iNr The number of rounds. This is
  *         determined by the size of the key.
  *  @param piIn The pointer to an integer array
  *         that contains the input ciphertext to
  *         be encrypted.
  *
  *  @param piOut The pointer to an integer array
  *         to store the plaintext.
  *  @param rgrgiKeySchedule The Key Schedule returned
  *         from the KeyExpansion function.
  *
  *  The Cipher operation is performed as follows:
  *  1. Copy contents from piIn to a 2-D integer array
  *     to represent the State known as rgrgiState
  *  2. Perform AddRoundKey operation on round iNr
  *  3. For each round from iNr-1 down to and
  *     including 1 perform the following:
  *         InvShiftRows then InvSubBytes
  *         operations on rgrgiState
  *         Then perform AddRoundKey operation
  *         on specific round
  8         Then InvMixColumns on rgrgiState
  *  4. Perform InvShiftRows and InvSubBytes on rgrgiState
  *  5. Perform AddRoundKey operation on round 0.
  *  6. Copy contetnst from rgrgiState to piOut.
  */
void InvCipher(int iNr,
               unsigned char *puchIn,
               unsigned char *puchOut,
               unsigned char rgrguchKeySchedule[NB*(iNr+1)][4]);
/**
  *  @brief Simple function to to print an integer array
  *
  *  @param iSize Size of data to print
  *  @param iOffset Offset to start printing
  *  @param piData pointer to integer array to print
  *
  *  This is acomplished by a simple for loop and printf
  *  of the hex representation.
  */
void PrintData(int iSize,int iOffset,unsigned char *puchData);
/**
  *  @brief Simple function to convert a char array
  *         to an integer array
  *
  *  @param iSize Number of char to convert to int
  *  @param pchArr Pointer to char array to convert
  *  @param piBytes Pointer to intever array to store in
  *
  *  This is acomplished by simply iterating through
  *  both arrays and casting pchArr[iIterator] to int
  *  and storing in piBytes.
  */
int CharToBytes(int iSize,char * pchArr,int * piBytes);
/**
  *  @brief Simple function to convert a integer array
  *         to an char array
  *
  *  @param iSize Number of char to convert to int
  *  @param piBytes Pointer to intever array to convert
  *  @param pchArr Pointer to char array to store in
  *  @param iOffset Offset from which to start the
  *         the conversion.
  *
  *  This is acomplished by simply iterating through
  *  both arrays and casting pchArr[iIterator] to int
  *  and storing in piBytes.
  */
int BytesToChar(int iSize,int * piBytes,char * pchArr,int iOffset);
/**
  *  @brief Simple function to fill the input integer array
  *         from a data vector
  *
  *  @param iSize Number of bytes to fill in
  *  @param iOffset Offset from which grab integers from the
  *         the data vector
  *  @param rgiIn Integer array to store data
  *  @param piData Pointer to integer array from which
  *         to grab integers
  *
  *  This accomplished through a simple for loop and storing
  *  piData[iOffset+iIterator] in rgiIn[iIterator].
  */
void FillIn(int iSize,int iOffset,int rgiIn[iSize],int * piData);
/**
  *  @brief Simple function to xor the values of two integer
  *         arrays
  *
  *  @param iSize Number of integers in both arrays to xor
  *  @param iOffset Offset from which to start xor operations
  *  @param rgiVectorPlain One integer array to xor. Result is
  *         stored in this integer array.
  *  @param rgiVector The other integer array to xor
  *
  *  This is acomplished through a simple for loop and store
  *  the result of xor operation of rgiVectorPlain[iIterator]
  *  rgiVector[iIterator] in rgiVectorPlain[iIterator].
  */
void XorVector(int iSize,int rgiVectorPlain[iSize],int rgiVector[iSize]);
/**
  *  @brief Function to perform AES Encryption in CBC mode
  *         with ISO 7816-4 padding
  *
  *  @param iKeySize Key Size in bytes
  *  @param iDataSize Size of data to encrypt
  *  @param rgiIv Integer array of the Initialization Vector
  *  @param rgiKey Integer array of the Key
  *  @param piData Pointer to an integer array that stores the
  *         data to encrypt
  *
  *  @return Pointer to an integer array named piEncData. Size
  *          of the encrypted data is stored in piEncData[0].
  *          Encrypted data is stored starting at piEncData[1].
  *
  *  This is acomplished by first calculating the padded size.
  *  Then realloc is used on piData to change the size to the
  *  padded size, and then malloc is used to initialize a piEncData,
  *  a pointer to an integer array to store the encrypted data to
  *  return. Then piData is padded based on ISO 7816-4.
  *  Then iNr and iNk are defined based on iKeySize. Next the
  *  KeyExpansion operation is performed. Then for each data
  *  block the following is performed:
  *      piData at offset (block number * block size) is copied
  *      to rgiIn (input vector).
  *      Next if the block number is 0, XorVector is performed as
  *      XorVector(block size,rgiIn,rgiIv).
  *      Otherwise, XorVector is performed as
  *      XorVector(block size,rgiIn,rgiOut).
  *      Next, Cipher operation is performed.
  *      Then, the integer array pointed to by Cipher is
  *      copied to piEncData.
  *  Finally the size of the encrypted data is stored at
  *  piEncData[0]. Then piData is freed and piEncData is returned.
  */
int *EncCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piData);
/**
  *  @brief Function to perform AES Encryption
  *
  *  @param iMode Mode of encryption to use
  *  @param iKeySize Key Size in bytes
  *  @param iDataSize Size of data to encrypt
  *  @param rgiIv Integer array of the Initialization Vector
  *  @param rgiKey Integer array of the Key
  *  @param piData Pointer to an integer array that stores the
  *         data to encrypt
  *
  *  @return Pointer to an integer array named piEncData. Size
  *          of the encrypted data is stored in piEncData[0].
  *          Encrypted data is stored starting at piEncData[1].
  *
  *  A switch is used to determine the mode to Encrypt piData,
  *  and then uses the appropriate encryption function. Then it
  *  returns the pointer returned by the encryption function.
  */
int *Enc(int iMode,int iKeySize,int iDataSize, int rgiIv[16],int rgiKey[iKeySize],int * piData);
/**
  *  @brief Function to perform AES Decryption in CBC mode
  *         with ISO 7816-4 padding
  *
  *  @param iKeySize Key Size in bytes
  *  @param iDataSize Size of data to decrypt
  *  @param rgiIv Integer array of the Initialization Vector
  *  @param rgiKey Integer array of the Key
  *  @param piEncData Pointer to an integer array that stores the
  *         data to decrypt
  *
  *  @return Pointer to an integer array named piData. Size
  *          of the decrypted data is stored in piData[0].
  *          Decrypted data is stored starting at piData[1].
  *  
  *  First the number of blocks is calculated based on block
  *  size and iDataSize. Then, the iNr and iNk are defined
  *  based on iKeySize. Then KeyExpansion is performed. Then
  *  the return pointer is initialized of size iDataSize+1.
  *  Then for each data block the following is performed:
  *      piData at offset (block number * block size) is copied
  *      to rgiIn (input vector).
  *      InvCipher operation is performed
  *      Next, if block number is 0, XorVector is performed as
  *      XorVector(block size,rgiOut,rgiIv), and rgiIn is copied
  *      to an integer array rgiLastIn.
  *      Else, xorVector is performed as XorVector(block size,rgiOut,rgiIn),
  *      and rgiIn is copied to an integer array rgiLastIn.
  *      Then, the contents in rgiOut are copied to piData at offset
  *      (block number * block size).
  *
  *  Then the length of the decrypted data stripped of padding
  *  is calculated, and realloc is used to change the size to
  *  that stripped of padding. Then the size of the decrypted
  *  data stripped of padding is stored in piData, and piEncData
  *  is freed.
  */
int *DecCBC(int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData);
/**
  *  @brief Function to perform AES Decryption
  *
  *  @param iMode Mode of decryption to use
  *  @param iKeySize Key Size in bytes
  *  @param iDataSize Size of data to decrypt
  *  @param rgiIv Integer array of the Initialization Vector
  *  @param rgiKey Integer array of the Key
  *  @param piEncData Pointer to an integer array that stores
  *         the data to decrypt
  *
  *  @return Pointer to an integer array named piData. Size
  *          of the decrypted data is stored in piData[0].
  *          Decrypted data is stored starting at piData[1].
  *
  *  A switch is used to determine the mode to Decrypt piEncData,
  *  and then uses the appropriate decryption function. Then it
  *  returns the pointer returned by the decryption function.
  */
int *Dec(int iMode,int iKeySize,int iDataSize,int rgiIv[16],int rgiKey[iKeySize],int * piEncData);
