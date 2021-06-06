/**
 * \file Decompression.c
 * \brief Decompresses the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"




FileBuffer getBufferCharFromTable(FILE* fileTable, int line, int sizeBuff)
{
    FileBuffer bufferChar;
    int c=0;
    int posBuffer=0;
    bufferChar.size = sizeBuff;
    bufferChar.text = (unsigned char*) malloc(bufferChar.size*sizeof(unsigned char));
    rewind(fileTable);

    for(int i=0; i<line; i++){
        wordWrapFile(fileTable);
    }
    c=fgetc(fileTable);
    while(posBuffer<bufferChar.size && c!=EOF){
        bufferChar.text[posBuffer]=c;
        posBuffer++;
        c=fgetc(fileTable);
    }

    return bufferChar;
    
}





HuffmanTreePtr createTreeFromBuffers(FileBuffer bufferPos, FileBuffer bufferChar)
{
    int posBufferPos=0;
    int posBufferChar=0;
    HuffmanTreePtr currentNode = NULL;
    HuffmanTreePtr previousNode=NULL;
    HuffmanTreePtr nextNode=NULL;
    HuffmanTreePtr head = NULL;
    uint8_t buffer=0;
    int sizeBuffer=0;
    int stop=0;
    uint8_t bit=0;
    uint8_t prevBit=1;

    //createNodeHuff(c, left, right, parent);
    head = createNodeHuff('\0', NULL, NULL, NULL);
    currentNode = head;
    printf("\n\n");
    while(posBufferPos<bufferPos.size && !stop){
        printf("a");
        buffer=bufferPos.text[posBufferPos];
        posBufferPos++;
        //printf("\nBUFFER    %d|%c\n", buffer, buffer);
        sizeBuffer=8;
        while(sizeBuffer!=0 && !stop){
            printf("\nSIZE_pos  %d      POS_pos %d\n", bufferPos.size, posBufferPos);
            printf("\nSIZE_ch  %d      POS_ch %d\n", bufferChar.size, posBufferChar);
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
                    //printf("0");
                    //We go back
                    if(prevBit==1){
                        currentNode->parent=previousNode;
                        currentNode->left=NULL;
                        currentNode->right=NULL;
                        currentNode->c=bufferChar.text[posBufferChar];
                        //printf("CURRENT NODE c :%d|%c\n", currentNode->c, currentNode->c);
                        posBufferChar++;
                    }
                    previousNode=currentNode;
                    currentNode=currentNode->parent;
                    break;

                case 1 : // We continue to the left (or right if we got back to the parent)
                    //printf("1");
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




int readTreeFromPos(HuffmanTreePtr* huffmanTreePos, uint8_t bit)
{
    switch (bit)
    {
        case 0 : // Go left
            printf("0");
            if((*huffmanTreePos)->left != NULL){
                (*huffmanTreePos) = (*huffmanTreePos)->left;
                if(((*huffmanTreePos)->left == NULL) && ((*huffmanTreePos)->right == NULL)) // We are at the end of a branch
                    return (*huffmanTreePos)->c;
                else
                    return -1;
            }
            else{
                // It's an error because we aren't at the end of a branch and the instruction is impossible (going left without having a branch at the left)
                fprintf(stderr, "\nERROR : Incorrect bit value\n");
                exit(EXIT_FAILURE);
            }
            break;
        case 1 : // Go right
            printf("1");
            if((*huffmanTreePos)->right != NULL){
                (*huffmanTreePos) = (*huffmanTreePos)->right;
                if(((*huffmanTreePos)->left == NULL) && ((*huffmanTreePos)->right == NULL)) // We are at the end of a branch
                    return (*huffmanTreePos)->c;
                else
                    return -1;
            }
            else{
                // It's an error because we aren't at the end of a branch and the instruction is impossible (going right without having a branch at the right)
                fprintf(stderr, "\nERROR : Incorrect bit value\n");
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "\nERROR : Incorrect bit value\n");
            exit(EXIT_FAILURE);
    }
}





/**
 * \fn void decompress(FILE* fileIn, FILE* bufferOut, FILE* fileTable)
 * \brief Decompresses fileIn in bufferOut by using fileTable
 * \param fileIn File that is being decompressed
 * \param bufferOut Decompressed buffer filled in this function
 * \param fileTable File containing the Huffman table (characters associated to their unique binary code)
 */


void decompress(FILE* fileIn, FileBuffer* bufferOut, HuffmanTreePtr huffmanTreeHead, int sizeFileIn)
{
    HuffmanTreePtr huffmanTreePos = huffmanTreeHead;
    uint8_t buffer=0;
    uint8_t bit;
    int progress=0;
    int c=-1;
    int sizeBuffer;
    int posBuffOut=0;
    bufferOut->text = (unsigned char*) malloc(sizeof(unsigned char)*sizeFileIn);
    bufferOut->size=sizeFileIn;
    rewind(fileIn);
    buffer=fgetc(fileIn);
    sizeBuffer=8;

    while(posBuffOut<sizeFileIn){
        while(sizeBuffer!=0 && posBuffOut<sizeFileIn){
            bit = buffer & (1<<(sizeBuffer-1));  // We use a mask to get the bit at the position sizebuffer-1 (between 2^0 and 2^7)
            if(sizeBuffer>1){
                bit >>= sizeBuffer-1;  // We shift to the right to get the value wanted completely to the right (and so we get a value equals to 0 or 1)
            }
            c = readTreeFromPos(&huffmanTreePos, bit);
            
            if(c!=-1){ // readTreeFromPos returned a character, we put it in bufferOut
                printf("\nc : %d|%c\n", c, c);
                bufferOut->text[posBuffOut]=c;
                posBuffOut++;
                huffmanTreePos=huffmanTreeHead;
                c=-1;
            }
            sizeBuffer--;
        }
        if(progress+5<((posBuffOut+1)*100)/sizeFileIn)
        {
            progress=(int)((((posBuffOut+1)*100)/sizeFileIn)/5)*5; //Display the progress of the current task
            printf("%d%%\n", progress);
        }

        buffer=fgetc(fileIn);
        sizeBuffer=8;
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
    FileBuffer bufferText;
    int indexBW=-1;
    int sizeFileIn=0;
    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    fileTable = fopen("table.txt", "rb");
    TESTFOPEN(fileTable);
    rewind(fileTable);

    printf("\nFilling buffers from the table...\n");
    FileBuffer bufferChar = getBufferCharFromTable(fileTable, 1, readNumberLine(fileTable, 3));
    rewind(fileTable);
    FileBuffer bufferPos = getBufferCharFromTable(fileTable, 0, readNumberLine(fileTable, 5));

    printf("\nGenerating the huffman tree from the table...\n");
    HuffmanTreePtr huffmanTree = createTreeFromBuffers(bufferPos, bufferChar);

    printf("\nGetting parameters from the table...\n");
    indexBW=readNumberLine(fileTable, 2);
    sizeFileIn=readNumberLine(fileTable, 4);
    FCLOSE(fileTable);
    printf("\nDecompression...\n");
    decompress(fileIn, &bufferText, huffmanTree, sizeFileIn);
    FCLOSE(fileIn);
    freeHuffmanTree(huffmanTree);

/*
    printf("\nBUFFER\n");
    for(int i=0; i<bufferText.size; i++){
        printf("%c", bufferText.text[i]);
    }
    printf("\n");
*/

    
    int sizeNameFileIn = strlen(fileNameIn);
    if(fileNameIn[sizeNameFileIn-4]=='.' && fileNameIn[sizeNameFileIn-3]=='b' && fileNameIn[sizeNameFileIn-2]=='i' && fileNameIn[sizeNameFileIn-1]=='n'){
        fileNameIn[sizeNameFileIn-4]='\0';  //The .bin is removed
    }

    //If the file already exists
    #if __linux__
    if(access(fileNameIn, F_OK)!=0){
        printf("The file \"%s\" already exists\n", fileNameIn);
        printf("Enter a name for the decompressed file : \n");
        getFileName(fileNameIn);
    }
    #endif
    #if __WIN32__
    if(_access(fileNameIn, 0)!=-1){
        printf("The file \"%s\" already exists\n", fileNameIn);
        printf("Enter a name for the decompressed file : \n");
        getFileName(fileNameIn);
    }
    #endif

    fileOut = fopen(fileNameIn, "wb+"); 
    TESTFOPEN(fileOut);
    if(indexBW>=0)
    {
        printf("\nDecoding Move To Front...\n");
        moveToFrontDecode(&bufferText);

        printf("\nDecoding Burrows Wheeler...\n");
        burrowsWheelerDecode(indexBW, bufferText, fileOut);
    }
    else
    {
        bufferToFile(bufferText, fileOut);
    }

    FCLOSE(fileOut);
    free(bufferText.text);
    printf("\nEnd of decompression\n");
}
