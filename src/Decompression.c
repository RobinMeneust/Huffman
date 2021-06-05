/**
 * \file Decompression.c
 * \brief Decompresses the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"




FileBuffer getBufferCharFromTable(FILE* fileTable)
{
    
}





HuffmanTreePtr createTreeFromFile(FILE* fileTable, FileBuffer bufferChar)
{
    int posBuffer=0;
    int sizeBuffer;
    HuffmanTreePtr currentNode = NULL;
    HuffmanTreePtr previousNode=NULL;
    HuffmanTreePtr nextNode=NULL;
    HuffmanTreePtr head = NULL;
    uint8_t buffer=0;
    int stop=0;
    uint8_t bit=0;
    uint8_t prevBit=1;
    rewind(fileTable);
    wordWrapFile(fileTable);
    wordWrapFile(fileTable);

    //createNodeHuff(c, left, right, parent);
    head = createNodeHuff('\0', NULL, NULL, NULL);
    currentNode = head;
    while(!feof(fileTable) && !stop){
        buffer=fgetc(fileTable);
        sizeBuffer=8;
        while(sizeBuffer!=0 && !stop){
            bit = buffer & (1<<(sizeBuffer-1));  // We use a mask to get the bit at the position sizebuffer-1 (between 2^0 and 2^7)
            if(sizeBuffer>1){
                bit >>= sizeBuffer-1;  // We shift to the right to get the value wanted completely to the right (and so we get a value equals to 0 or 1)
            }
            if(bit==0 && currentNode==head){ // We are at the end of the buffer
                stop=1;
            }
            switch (bit)
            {
            case 0 : // We go back to the parent
                //We go back
                if(prevBit==1){
                    currentNode->parent=previousNode;
                    currentNode->c=bufferChar.text[posBuffer];
                    posBuffer++;
                }
                currentNode->left=NULL;
                currentNode->right=NULL;
                previousNode=currentNode;
                currentNode=currentNode->parent;
                break;

            case 1 : // We continue to the left (or right if we got back to the parent)
                if(prevBit==0){ // = We got back to the parent
                    //We go to the right
                    nextNode = createNodeHuff('\0', NULL, NULL, currentNode);
                    currentNode->right = nextNode;
                    previousNode = currentNode;
                    currentNode = currentNode->right;
                }
                else{
                    //We go to the left
                    nextNode = createNodeHuff('\0', NULL, NULL, currentNode);
                    currentNode->left = nextNode;
                    previousNode = currentNode;
                    currentNode = currentNode->left;
                }
                break;

            default :
                fprintf(stderr, "\nERROR : Huffman tree creation issue\n");
                exit(EXIT_FAILURE);
                break;
            }
            prevBit=bit;
            sizeBuffer--;
        }
    }
    return head;
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
    int sizeFileIn = readNumberLine(fileTable, 1);
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
        fprintf(stderr, "ERROR : Could not find the code in the table");
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

    FileBuffer bufferChar = getBufferCharFromTable(fileTable);

    HuffmanTreePtr huffmanTree = createTreeFromFile(fileTable, bufferChar);

    FCLOSE(fileTable);
    indexBW=readNumberLine(fileTable, 0);

    printf("\nDecompression...\n");
    FileBuffer buffertext;
    decompress(fileIn, &buffertext, huffmanTree);
    FCLOSE(fileIn);
    FCLOSE(fileTable);
    
    int sizeNameFileIn = strlen(fileNameIn);
    if(fileNameIn[sizeNameFileIn-4]=='.' && fileNameIn[sizeNameFileIn-3]=='b' && fileNameIn[sizeNameFileIn-2]=='i' && fileNameIn[sizeNameFileIn-1]=='n'){
        fileNameIn[sizeNameFileIn-4]='\0';  //The .bin is removed
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
    if(indexBW>=0)
    {
        printf("\nDecoding Move To Front...\n");
        moveToFrontDecode(&buffertext);

        printf("\nDecoding Burrows Wheeler...\n");
        burrowsWheelerDecode(indexBW, buffertext, fileOut);
    }
    else
    {
        bufferToFile(buffertext, fileOut);
    }
    FCLOSE(fileOut);
    free(buffertext.text);
    printf("\nEnd of decompression\n");
}
