#include "sprint1.h"
/**
  *  @brief Kernel entrypoint
  *
  *  @param argc Number of arguments
  *  @param argv pointer to the arguments
  *
  *  @return status code (0 for success 1 for error)
  *  
  *  Program recieve key size as a command line
  *  argument
  */
int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Too Many Arguments.\n");
        return 1;
    } else if (argc < 2) {
        printf("Insufficient Arguments.\n");
        return 1;
    }
    /**  @var chBuff Char used for dynamic user input string*/
    char chBuff = 0;
    /**  @var pchString Pointer to char array for storing
      *       user input
      */
    char *pchString = malloc(sizeof(char));
    /**  @var pchEncString Pointer to char array for storing
      *       string representation of the encrypted data
      */
    char *pchEncString = malloc(sizeof(char));
    /**  @var iIterator Integer for iterating for user input*/
    int iIterator = 0;
    /**  @var rgiIv Integer array for Initialization Vector*/
    unsigned char rguchIv[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    /**  @var piKey Pointer to integer array for storing a
      *       dynamically allocated key based on random values
      */
    unsigned char *puchKey = malloc(sizeof(unsigned char));
    /**  @var iKeySize Integer to store the Key Size given by user*/
    int iKeySize = atoi(argv[1]);
    /**  @var iRetSize Integer representing the size of the
      *        encrypted data
      */
    int iRetSize = 0;
    unsigned char *puchOutEncData = NULL;
    /** switch for usable key sizes versus incorrect key sizes*/
    switch (iKeySize) {
        case 128:
        case 192:
        case 256:
            // Setup randomly generate key
            iKeySize = iKeySize / 8;
            puchKey = realloc(puchKey, iKeySize * sizeof(unsigned char));
            for (int iInIterator = 0; iInIterator < iKeySize; iInIterator++) {
                puchKey[iInIterator] = rand() / (RAND_MAX / 256 + 1);
            }
            printf("Enter Message\n");
            while ((chBuff = getchar()) != '\n') {
                pchString = (char *)realloc(pchString, iIterator * sizeof(char) + 1);
                pchString[iIterator++] = chBuff;
            }
            /**  @var piData Pointer to integer array for storing
              *       integer values of user defined string
              */
            /**  @var piOutEncData Pointer to store returned value
              *       from Enc()
              */
            int iRetSize = Enc(0, rguchIv, iKeySize, puchKey, iIterator,
                               (unsigned char *)pchString, &puchOutEncData);

            printf("Encrypted Result is:\n%s\nLength: %d\nKey: ",
                   puchOutEncData, iRetSize);
            PrintData(iKeySize, 0, puchKey);
            printf("IV: ");
            PrintData(16, 0, rguchIv);
            printf("Decrypt Result? (y/n)\n");
            char c = getchar();

            if (c == 'y' || c == 'Y') {
                unsigned char *puchOutDecData = NULL;
                /**
                  *  @var piPostDec Pointer to store returned value
                  *       from Dec()
                  */
                iRetSize = Dec(0, rguchIv, iKeySize, puchKey, iRetSize,
                               puchOutEncData, &puchOutDecData);
                printf("%s\n", puchOutDecData);
                free(puchOutDecData);
            }

            break;
        default:
            printf("Incorrect entry for key size. Exiting...\n");
            return 1;
            break;
    }

    free(puchKey);
    free(pchString);
    free(pchEncString);
    return 0;
}
