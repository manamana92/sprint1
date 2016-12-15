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
	printf("%02x\n",bytes[i]);
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
        printf("%d before: %d\t",i,vectorPlain[i]);
        vectorPlain[i]=vectorPlain[i]^vector[i];
	printf("after: %d\n",vectorPlain[i]);
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
	    printf("%02x\n",in[j]);
        }
	if(i==0){
	    printData(size,in);
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
int main(void){
    char *arr = "Hello World";
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
        data = (int *)realloc(data,paddedSize*sizeof(int));
	data[sizeOfData]=0x80;
        for(int i = sizeOfData+1;i<paddedSize;i++){
            data[i]=0;
        }
    }
    enc(0,16,paddedSize,iv,key,data,encData);

    printData(paddedSize,encData);
    /**free(arr);*/
    free(data);
    
}
