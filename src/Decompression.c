/**
 * \file Decompression.c
 * \brief Decompresses the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int initializePossibleElementsArray(int* possibleElementsArray, FILE* fileTable)
 * \brief Initializes possibleElementsArray so that only cells asociated to characters that exist in fileTable are equal to 1 (and the others to 0)
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \param fileTable File containing the Huffman table (characters associated to their unique binary code)
 * \return Amount of non-null cells
*/

int initializePossibleElementsArray(int* possibleElementsArray, FILE* fileTable)
{
    int nbElements=0;
    unsigned char c;
    rewind(fileTable);
    wordWrapFile(fileTable);
    wordWrapFile(fileTable);
    wordWrapFile(fileTable);
    c=fgetc(fileTable);
    for(int i=0; i<N_ASCII; i++){
        possibleElementsArray[i]=0;
    }
    while(!feof(fileTable)){
        possibleElementsArray[c]=1;
        nbElements++;
        wordWrapFile(fileTable);
        c=fgetc(fileTable);
    }
    return nbElements;
}

/**
 * \fn unsigned char seekFirstPositiveIndex(int* possibleElementsArray)
 * \brief Seeks the first index corresponding to a non-null cell in the given array
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \return First non-null index (which is also a character) of possibleElementsArray
*/


unsigned char seekFirstPositiveIndex(int* possibleElementsArray)
{
    for(unsigned char i=0; i<N_ASCII; i++){
        if(possibleElementsArray[i]==1){
            return i;
        }
    }
    printf("ERROR : Array without any non-null value");
    exit(EXIT_FAILURE);
}

/**
 * \fn refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements)
 * \brief Refreshes possibleElementsArray depending the bit and the position given (by comparing the value of the bit at this position in the table to this bit)
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \param table File containing the Huffman table (characters associated to their unique binary code)
 * \param bit Bit given, compared to the binary value at the given position in the table
 * \param position Position of the bit read form the code in the table, positive value corresponding to the first value of a line. The higher this number get, the more we shift to the right
 * \param nbElements Number of non-null cells in possibleElementsArray
*/

void refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements)
{
    rewind(table);
    wordWrapFile(table);
    wordWrapFile(table);
    wordWrapFile(table);
    unsigned char c = fgetc(table);
    unsigned char c_table;
    int i=0;

    while(!feof(table)){
        if(possibleElementsArray[c]!=0){
            c_table=fgetc(table);
            i=0;
            while(i<position && c_table!='\n' && !feof(table)){
                c_table=fgetc(table);
                i++;
            }
            if(c_table-'0'!=bit){
                possibleElementsArray[c]=0;
                (*nbElements)--;
            }
        }
        wordWrapFile(table);
        c = fgetc(table);
    }
}



/**
 * \fn void decompress(FILE* fileIn, FILE* bufferOut, FILE* fileTable)
 * \brief Decompresses fileIn in bufferOut by using fileTable
 * \param fileIn File that is being decompressed
 * \param bufferOut Decompressed buffer filled in this function
 * \param fileTable File containing the Huffman table (characters associated to their unique binary code)
*/


void decompress(FILE* fileIn, FileBuffer* bufferOut, FILE* fileTable)
{
    int sizeFileIn = readNumberLine(fileTable, 2);
    int nbInsertions=0; // Number of characters that we have to insert (size of the initial uncompressed file)
    uint8_t buffer=0;
    uint8_t bit;
    int progress=0;
    int sizeBuffer;
    int position;
    int possibleElementsArrayPossibles[N_ASCII];  // contains 0 or 1 : 1 = the sequence matches or 0 in the other case
    int nbElementsPossibles = N_ASCII;
    int pos=0;
    bufferOut->text = (unsigned char*) malloc(sizeof(unsigned char)*sizeFileIn);
    bufferOut->size=sizeFileIn;
    rewind(fileIn);
    buffer=fgetc(fileIn);
    position=0;
    sizeBuffer=8;
    nbElementsPossibles = initializePossibleElementsArray(possibleElementsArrayPossibles, fileTable);
    while(nbInsertions<sizeFileIn && nbElementsPossibles!=0){
        while(sizeBuffer!=0 && nbElementsPossibles!=0 && nbInsertions<sizeFileIn){
            bit = buffer & (1<<(sizeBuffer-1));  // We use a mask to get the bit at the position sizebuffer-1 (between 2^0 and 2^7)
            if(sizeBuffer>1){
                bit >>= sizeBuffer-1;  // We shift to the right to get the value wanted completely to the right (and so we get a value equals to 0 or 1)
            }
            refreshPossibleElementsArray(possibleElementsArrayPossibles, fileTable, bit, position, &nbElementsPossibles);
            position++;
            if(nbElementsPossibles==1){ // There is only possibility left, so it's the (decompressed) character that we have to insert
                unsigned char c=seekFirstPositiveIndex(possibleElementsArrayPossibles);
                bufferOut->text[pos]=c;
                pos++;
                position=0;
                nbElementsPossibles = initializePossibleElementsArray(possibleElementsArrayPossibles, fileTable);
                nbInsertions++;
            }
            sizeBuffer--;
        }

        if(progress+5<((nbInsertions+1)*100)/sizeFileIn)
        {
            progress=(int)((((nbInsertions+1)*100)/sizeFileIn)/5)*5; //Display the progress of the current task
            printf("%d%%\n", progress);
        }

        buffer=fgetc(fileIn);
        sizeBuffer=8;
    }

    if(nbElementsPossibles==0){
        exit(EXIT_FAILURE);
    }
}


/**
 * \fn void decompressMain(char* fileNameIn)
 * \brief Main function for decompression : calls required functions to the decompression of the file whose name is given to the function
 * \param fileNameIn Name of the file that is being decompressed
*/


void decompressMain(char* fileNameIn)
{
    FILE* fileIn;
    FILE* fileOut;
    FILE* fileTable;
    int indexBW;

    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    fileTable = fopen("table.txt", "rb");
    TESTFOPEN(fileTable);

    indexBW=readNumberLine(fileTable, 0);

    printf("\nDecompression...\n");
    FileBuffer buffertext;
    decompress(fileIn, &buffertext, fileTable);
    FCLOSE(fileIn);
    
    int option=readNumberLine(fileTable, 1);
    printf("opt : %d\n", option);
    FCLOSE(fileTable);
    
    int fileSizeNameEntree = seekStringSize(fileNameIn);
    if(fileNameIn[fileSizeNameEntree-4]=='.' && fileNameIn[fileSizeNameEntree-3]=='b' && fileNameIn[fileSizeNameEntree-2]=='i' && fileNameIn[fileSizeNameEntree-1]=='n'){
        fileNameIn[fileSizeNameEntree-4]='\0';  //The .bin is removed
    }

    //If the file already exists
    #if __linux__
    if(access(fileNameIn, F_OK)!=0){
        printf("ERROR : The file \"%s\" already exists", fileNameIn);
        printf("Enter a name for the decompressed file : \n");
        getFileName(fileNameIn);
    }
    #endif
    #if __WIN32__
    if(_access(fileNameIn, 0)!=-1){
        printf("ERROR : The file \"%s\" already exists", fileNameIn);
        printf("Enter a name for the decompressed file : \n");
        getFileName(fileNameIn);
    }
    #endif

    fileOut = fopen(fileNameIn, "wb+"); 
    TESTFOPEN(fileOut);
    if(option==1)
    {
        printf("\nInverse of Move To Front...\n");
        moveToFrontDecode(&buffertext);

        printf("\nInverse of Burrows Wheeler...\n");
        burrowsWheelerDecode(indexBW, buffertext, fileOut);
    }
    else
    {
        bufferToFile(buffertext,fileOut);
    }
    FCLOSE(fileOut);
    free(buffertext.text);
    printf("\nEnd of decompression\n");
}
