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
    int in[size];
    int out[size];
    int nb=4,nr,nk;

    /**Data Padding 80 00 ...*/
    int dataSizeModBlock=(dataSize+1)%size;
    int paddedSize=dataSize+(size-dataSizeModBlock)+1;
    int numBlocks = paddedSize/size;
    printf("Data Size: %d, Mod Size: %d, Padded Size: %d",dataSize,dataSizeModBlock,paddedSize);
    data=(int *)realloc(data,paddedSize*sizeof(int));
    data[dataSize]=0x80;
    encData=(int *)realloc(encData,paddedSize*sizeof(int));
    for(int i = dataSize+1;i<paddedSize;i++){
        data[i]=0;
    }
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
    return paddedSize;
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
    printData(dataSize,data);
    /**Handle Padding 80 00 00 ...*/
    int lastByte = dataSize-1;
    while(data[lastByte]!=0x80&&lastByte>0){
        lastByte--;
    }
    data=(int *)realloc(data,(lastByte+1)*sizeof(int));
    return lastByte+1;
}
int enc(int mode,int size,int dataSize, int iv[size],int key[size],int * data,int * encData){
    int retVal = 0;
    switch(mode){
        case 0:
            retVal = encCBC(size,dataSize,iv,key,data,encData);
            printf("Outside Size: %d\n",retVal);
            break;
        default:
	    break;
    }
    return retVal;
}
int dec(int mode,int size,int dataSize,int iv[size],int key[size],int * encData,int * data){
    int retVal = 0;
    switch(mode){
        case 0:
            retVal = decCBC(size,dataSize,iv,key,encData,data);
            printf("Outside Size: %d\n",dataSize);
	    break;
        default:
            break;
    }
    return retVal;
}
int main(void){
    char *arr = "Hello my name is Chris";
    int sizeOfData = strlen(arr);
    int *data=(int *)malloc(sizeOfData*sizeof(int));
    charToBytes(sizeOfData,arr,data);
    int iv[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int key[]={0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c};

    int sizeOfBlock=16;

    /**int sizeOfDataModBlock = sizeOfData%sizeOfBlock;
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
    }*/
    int *encData=(int *)malloc(sizeOfData*sizeof(int));
    int paddedSize = enc(0,sizeOfBlock,sizeOfData,iv,key,data,encData);
    printData(paddedSize,encData);
    int *postDec=(int *)malloc(paddedSize*sizeof(int));
        
    int postSize = dec(0,sizeOfBlock,paddedSize,iv,key,encData,postDec);
    /**int lastByte = paddedSize-1;
    while(postDec[lastByte]!=0x80&&lastByte>0){
        lastByte--;
    }
    printf("\n%02x\n",postDec[lastByte]);

    postDec=(int *)realloc(postDec,(lastByte+1)*sizeof(int));*/
    /**int sizeOut = sizeof(*postDec)/sizeof(int);*/
    char *postArr=malloc((postSize)*sizeof(char));
    bytesToChar(postSize,postDec,postArr);
    postArr[postSize-1]='\0';
    printf("Size of char arr %d\n",postSize);
    puts(postArr);
    /**free(arr);*/
    free(data);
    
}
