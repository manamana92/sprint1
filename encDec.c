#include "sprint1.h"
void printData(int size,int * data){
    for(int i = 0;i<size;i++){
        printf("%02x ",data[i]);
    }
    printf("\n");
}
int charToBytes(int size,char * arr,int * bytes){
    for(int i = 0;i<size;i++){
        bytes[i]=(int)arr[i];
    }

    return 0;
}
int bytesToChar(int size,int * bytes,char * arr){
    for(int i = 0;i<size;i++){
        arr[i]=(char)bytes[i];
    }
    return 0;
}
void fillIn(int size,int offset,int in[size],int * data){
    for(int i=0;i<size;i++){
        in[i]=data[offset+i];
    }
}
void xorVector(int size,int vectorPlain[size],int vector[size]){
    for(int i = 0;i<size;i++){
        vectorPlain[i]=vectorPlain[i]^vector[i];
    }
}
int encCBC(int size,int dataSize,int iv[size],int key[size],int * data,int * encData){
    int numBlocks = dataSize/size;
    int in[size];
    int out[size];
    /**int encData[totalSize];*/
    int nb=4,nr,nk;
    switch(size){
        case 16:
            nr = 10;
            nk = 4;
            break;
        case 24:
            nr = 12;
            nk = 6;
            break;
        case 32:
            nr = 14;
            nk = 8;
            break;
        default:
	    return 1;
            break;
    }
    int keySchedule[nb*(nr+1)][4];
    keyExpansion(nb,nr,nk,key,keySchedule);


    for(int i = 0;i<numBlocks;i++){
        for(int j=0;j<size;j++){
            in[j]=data[(i*size)+j];
        }
	printf("Block %d",i);
	printData(size,in);
	if(i==0){
            xorVector(size,in,iv);
        }else{
            xorVector(size,in,out);
        }
        cipher(nb,nr,in,out,keySchedule);

	for(int k=0;k<size;k++){
            encData[(i*size)+k]=out[k];
        }

    }
    return 0;
}
int decCBC(int size,int dataSize,int iv[size],int key[size],int * encData, int * data){
    int numBlocks=dataSize/size;
    int in[size];
    int out[size];
    int lastIn[size];
    int nb=4,nr,nk;
    switch(size){
        case 16:
            nr = 10;
            nk = 4;
            break;
        case 24:
            nr = 12;
            nk = 6;
            break;
        case 32:
            nr = 14;
            nk = 8;
            break;
        default:
            return 1;
    }
    int keySchedule[nb*(nr+1)][4];
    keyExpansion(nb,nr,nk,key,keySchedule);

    for(int i = 0;i<numBlocks;i++){
        for(int j = 0;j<size;j++){
            in[j]=encData[(i*size)+j];
        }
        invCipher(nb,nr,in,out,keySchedule);

	if(i==0){
            xorVector(size,out,iv);
	    for(int k=0;k<size;k++){
                lastIn[k]=in[k];
            }
        }else{
            xorVector(size,out,lastIn);
            for(int k=0;k<size;k++){
                lastIn[k]=in[k];
            }
        }

	for(int k = 0;k<size;k++){
            data[(i*size)+k]=out[k];
        }
    }
    return 0;
}
int enc(int mode,int size,int dataSize, int iv[size],int key[size],int * data,int * encData){
    switch(mode){
        case 0:
            encCBC(size,dataSize,iv,key,data,encData);
            break;
        default:
	    break;
    }
    return 0;
}
int dec(int mode,int size,int dataSize,int iv[size],int key[size],int * encData,int * data){
    switch(mode){
        case 0:
            decCBC(size,dataSize,iv,key,encData,data);
	    break;
        default:
            break;
    }
    return 0;
}
int main(void){
    char *arr = "Hello my name is Chris";
    int sizeOfData = strlen(arr);
    int *data=(int *)malloc(sizeOfData*sizeof(int));
    charToBytes(sizeOfData,arr,data);
    int iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int key[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

    int sizeOfBlock=16;

    int sizeOfDataModBlock = sizeOfData%sizeOfBlock;
    int paddedSize = sizeOfData+(sizeOfBlock-sizeOfDataModBlock);

    printf("Size Block: %d, Size Data: %d,Modded Size %d, Padded Size: %d\n",sizeOfBlock,sizeOfData,sizeOfDataModBlock,paddedSize);
    int *encData=(int *)malloc(paddedSize*sizeof(int));
    if(sizeOfDataModBlock!=0){
    printData(paddedSize,encData);
        data = (int *)realloc(data,paddedSize*sizeof(int));
	data[sizeOfData]=0x80;
        for(int i = sizeOfData+1;i<paddedSize;i++){
            data[i]=0;
        }
    }
    enc(0,16,paddedSize,iv,key,data,encData);

    int *postDec=(int *)malloc(paddedSize*sizeof(int));

    dec(0,16,paddedSize,iv,key,encData,postDec);
    printf("After Decryption\n");
    printData(paddedSize,postDec);
    int lastByte = paddedSize-1;
    while(postDec[lastByte]!=0x80&&lastByte>0){
        lastByte--;
    }
    printf("\n%02x\n",postDec[lastByte]);

    postDec=(int *)realloc(postDec,(lastByte+1)*sizeof(int));
    char *postArr=malloc((lastByte+1)*sizeof(char));
    bytesToChar(lastByte,postDec,postArr);
    postArr[lastByte]='\0';
    puts(postArr);
    /**free(arr);*/
    free(data);
    
}
