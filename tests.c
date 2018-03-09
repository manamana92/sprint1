#include "sprint1.h"
/*void testNullEnc(void){
    char *pchString = malloc(14*sizeof(char));
    pchString[13]='\0';
    int *piData = malloc(14*sizeof(int));
    PrintData(14,0,piData);
    CharToBytes(14,pchString,piData);
    PrintData(14,0,piData);

    int iSizeData = sizeof(&piData);

    int iSizeChar = sizeof(&pchString);

    int key128[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};
    int rgiIv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int *piEncData = Enc(0,16,14,rgiIv,key128,piData);
    int iRetSize = piEncData[0];
    char *pchEncString = (char *)malloc((iRetSize+1)*sizeof(char));
    BytesToChar(iRetSize,piEncData,pchEncString,1);
    pchEncString[iRetSize]='\0';


    printf("Encrypted Result is:\n%s\n",pchEncString);

    
    printf("Size of piData %d\nSize of pchString %d\n",iSizeData,iSizeChar);
}*/
void testRotArr(){
    printf("Test case for size greater than size of array\n");
    unsigned char rguchTest[] = {0x01,0x12,0x23,0x34};
    RotWord(rguchTest);
    PrintData(4,0,rguchTest);
    printf("Null test case\n");
    /*int *piTest=(int *)malloc(5*sizeof(int));
    PrintData(5,0,piTest);
    RotWord(piTest);
    PrintData(5,0,piTest);*/

    
}
void testSubByte(){
    unsigned char test = 0xff;
    printf("Original: %x Substituted: %x\n",0xff,SubByte(test));
}
void testSubWord(){
    unsigned char test[] = {0xff,0xff,0xff,0xff};
    printf("Original:");
    for(int i = 0; i < 4; i++){
        printf(" %x", test[i]);
    }
    SubWord(test);
    printf("Substituted:");
    for(int j = 0; j < 4; j++){
        printf(" %x",test[j]);
    }
    printf("\n");
}
void testReverse(){
    unsigned char test[] = {0x01,0x02,0x03,0x04};
    printf("Original:");
    PrintData(4,0,test);
    Reverse(test,4);
    printf("Reversed:");
    PrintData(4,0,test);
    printf("\n");
}
void testMult(){
    unsigned char test1 = 0x0;
    unsigned char test2 = 0x0;
    int res = 0;
    for(int i = 0; i<255; i++){
        for(int j = 0; j<255; j++){
            res = Mult((unsigned char)(test1+i),(unsigned char)(test2+j));
            if(res>255){
                printf("Over result: %x \n",res);
	    }
	}
    }
}
void testKeyExpansion(){
    unsigned char rguchKey[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c};
    unsigned char rgrguchKeySchedule[44][4] = {{0x2b,0x7e,0x15,0x16},
                                               {0x28,0xae,0xd2,0xa6},
					       {0xab,0xf7,0x15,0x88},
					       {0x09,0xcf,0x4f,0x3c},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0}};
    KeyExpansion(10,4,rguchKey,rgrguchKeySchedule);
    for(int i = 0;i<44;i++){
	printf("Round %d\t",i);
        for(int j =0;j<4;j++){
            printf("%02x",rgrguchKeySchedule[i][j]);
        }
        printf("\n");
    }

}
void testInToState(){
    unsigned char arr[] = {0x01,0x02,0x03,0x04,
	                   0x05,0x06,0x07,0x08,
			   0x09,0x0a,0x0b,0x0c,
			   0x0d,0x0e,0x0f,0x10};
    unsigned char state[4][4];
    InToState(arr, state);
    PrintState(state);
}
void testStateToOut(){
    unsigned char state[4][NB] = {{0x01,0x05,0x09,0x0d},
	                          {0x02,0x06,0x0a,0x0e},
                                  {0x03,0x07,0x0b,0x0f},
                                  {0x04,0x08,0x0c,0x10}};
    unsigned char arr[16];
    StateToOut(state, arr);
    PrintData(16,0,arr);
}
void testAddRoundKey(){
    unsigned char rguchKey[16] = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,
        0x09,0xcf,0x4f,0x3c};
    unsigned char rgrguchState[4][NB] = {{0,0,0,0},
                                         {0,0,0,0},
                                         {0,0,0,0},
                                         {0,0,0,0}};
    unsigned char rgrguchKeySchedule[44][4] = {{0x2b,0x7e,0x15,0x16},
                                               {0x28,0xae,0xd2,0xa6},
					       {0xab,0xf7,0x15,0x88},
					       {0x09,0xcf,0x4f,0x3c},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0}};
    KeyExpansion(10,4,rguchKey,rgrguchKeySchedule);
    AddRoundKey(10, 0, rgrguchState, rgrguchKeySchedule);
    PrintState(rgrguchState);
}
void testsubState(){
    unsigned char rgrguchState[4][NB] = {{0xff,0xff,0xff,0xff},
                                         {0xff,0xff,0xff,0xff},
                                         {0xff,0xff,0xff,0xff},
                                         {0xff,0xff,0xff,0xff}};
    SubState(rgrguchState);
    PrintState(rgrguchState);
}
void testShiftRows(){
    unsigned char arr[] = {0x01,0x02,0x03,0x04,
	                   0x05,0x06,0x07,0x08,
                           0x09,0x0a,0x0b,0x0c,
                           0x0d,0x0e,0x0f,0x10};
    unsigned char state[4][NB] = {{0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0}};
    InToState(arr, state);
    printf("Original:\n");
    PrintState(state);
    ShiftRows(state);
    printf("Shifted:\n");
    PrintState(state);
}
void testMixColumns(){
    unsigned char arr[] = {0xd4,0xbf,0x5d,0x30,
                           0xe0,0xb4,0x52,0xae,
                           0xb8,0x41,0x11,0xf1,
                           0x1e,0x27,0x98,0xe5};
    unsigned char state[4][NB] = {{0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0}};

    InToState(arr, state); 
    printf("Original:\n");
    PrintState(state);
    MixColumns(state);
    printf("Shifted:\n");
    PrintState(state);
}
void testCipher(){

    unsigned char rguchKey[16] = {0x2b,0x7e,0x15,0x16,
                                  0x28,0xae,0xd2,0xa6,
                                  0xab,0xf7,0x15,0x88,
                                  0x09,0xcf,0x4f,0x3c};
    unsigned char rgrguchKeySchedule[44][4] = {{0,0,0,0},
                                               {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0}};
    KeyExpansion(10,4,rguchKey,rgrguchKeySchedule);
    unsigned char inputArray[] = {0x32, 0x43, 0xf6, 0xa8,
                                  0x88, 0x5a, 0x30, 0x8d,
                                  0x31, 0x31, 0x98, 0xa2,
                                  0xe0, 0x37, 0x07, 0x34};
    unsigned char outputArray[16] = {0,0,0,0,0,0,0,0,
                                     0,0,0,0,0,0,0,0}; 
    unsigned char state[4][NB] = {{0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0}};
    InToState(inputArray, state);
    printf("Original:\n");
    PrintState(state);

    Cipher(10,inputArray,outputArray,rgrguchKeySchedule);
    InToState(outputArray, state);
    printf("After Cipher:\n");
    PrintState(state);
    
}
void testInvShiftRows(){
    unsigned char arr[] = {0x01,0x02,0x03,0x04,
	                   0x05,0x06,0x07,0x08,
                           0x09,0x0a,0x0b,0x0c,
                           0x0d,0x0e,0x0f,0x10};
    unsigned char state[4][NB] = {{0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0}};
    InToState(arr, state);
    printf("Original:\n");
    PrintState(state);
    InvShiftRows(state);
    printf("Shifted:\n");
    PrintState(state);
}
    
void testInvSubState(){
    unsigned char rgrguchState[4][NB] = {{0x3d,0x3d,0x3d,0x3d},
                                         {0x3d,0x3d,0x3d,0x3d},
                                         {0x3d,0x3d,0x3d,0x3d},
                                         {0x3d,0x3d,0x3d,0x3d}};
    InvSubBytes(rgrguchState);
    PrintState(rgrguchState);
}
void testInvCipher(){

    unsigned char rguchKey[16] = {0x00,0x01,0x02,0x03,
                                  0x04,0x05,0x06,0x07,
                                  0x08,0x09,0x0a,0x0b,
                                  0x0c,0x0d,0x0e,0x0f};
    unsigned char rgrguchKeySchedule[44][4] = {{0,0,0,0},
                                               {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0},
					       {0,0,0,0}};
    KeyExpansion(10,4,rguchKey,rgrguchKeySchedule);
    unsigned char inputArray[] = {0x00, 0x11, 0x22, 0x33,
                                  0x44, 0x55, 0x66, 0x77,
                                  0x88, 0x99, 0xaa, 0xbb,
                                  0xcc, 0xdd, 0xee, 0xff};
    unsigned char encArray[16] = {0,0,0,0,0,0,0,0,
                                  0,0,0,0,0,0,0,0}; 
    unsigned char outputArray[16] = {0,0,0,0,0,0,0,0,
                                     0,0,0,0,0,0,0,0}; 
    unsigned char state[4][NB] = {{0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0},
                                  {0,0,0,0}};
    InToState(inputArray, state);
    printf("Original:\n");
    //PrintState(state);
    PrintData(16,0,inputArray);

    Cipher(10,inputArray,encArray,rgrguchKeySchedule);
    InToState(outputArray, state);
    printf("After Cipher:\n");
    //PrintState(state);
    PrintData(16,0,encArray);
    
    InvCipher(10, encArray, outputArray, rgrguchKeySchedule);
    printf("After Cipher:\n");
    //PrintState(state);
    PrintData(16,0,outputArray);
}

int main(void){
    printf("%d",INT_MIN);
    //testNullEnc();
    testRotArr();
    testSubByte();
    testSubWord();
    testReverse();
    testMult();
    testKeyExpansion();
    testInToState();
    testStateToOut();
    testAddRoundKey();
    testsubState();
    testShiftRows();
    testMixColumns();
    testCipher();
    testInvShiftRows();
    testInvSubState();
    testInvCipher();
    return 0;
}
