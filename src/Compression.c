/**
 * \file Compression.c
 * \brief Compresses the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
 */

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"


/**
 * \fn void freeHuffmanTable(HuffmanTableCell* huffmanTable, int sizeHuffmanTable)
 * \brief Frees the memory used by huffmanTable and its linked lists (of the field code)
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of huffmanTable
 */


void freeHuffmanTable(HuffmanTableCell* huffmanTable, int sizeHuffmanTable)
{
    PtrlistCode p = NULL;
    for(int i=0; i<sizeHuffmanTable; i++){
        while(huffmanTable[i].code!=NULL){
            p = huffmanTable[i].code;
            huffmanTable[i].code = huffmanTable[i].code->next;
            free(p);
        }
    }
    free(huffmanTable);
}



/**
 * \fn void compress(FileBuffer bufferBW, FILE* fileOut, FileBuffer bufferTable)
 * \brief Compresses bufferBW by using the Huffman table
 * \param bufferBW Buffer that is being compressed
 * \param fileOut File compressed filled in this function by using the Huffman coding
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Number of unique elements in the initial file (after the application of the extensions). Size of the array huffmanTable
 */

void compress(FileBuffer bufferBW, FILE* fileOut, HuffmanTableCell* huffmanTable, int sizeHuffmanTable)
{
    uint8_t buffer=0;   // Byte used to contain the binary code before being inserted in the file fileOut
    FileBuffer bufferOut; // Used to write in fileOut
    bufferOut.text = (unsigned char*) malloc(sizeof(unsigned char)*BUFFER_SIZE);
    bufferOut.size=0;
    int filling=0;
    int progress=0;
    unsigned char c, c_table;
    rewind(fileOut);
    int posIn=0;
    int posTable=0;
    PtrlistCode l=NULL;
    while(posIn<bufferBW.size){
        c = bufferBW.text[posIn];
        c_table = huffmanTable[posTable].c;
        while(c_table!=c && posTable<sizeHuffmanTable){
            posTable++;
            c_table = huffmanTable[posTable].c;
        }
        if(c_table!=c){
            printf("ERROR : Character not found in the table : %c|%d", c, c);
            exit(EXIT_FAILURE);
        }

        l = huffmanTable[posTable].code;
        while(l!=NULL){
            switch(l->value){
                case '0': buffer <<= 1; break;  //We shift bits to the left, then we add the bit (0 here) to the right
                case '1': buffer <<= 1; buffer|=0b1; break; //We shift bits to the left, then we add the bit (1 here) to the right, we use a mask to do so
                default : fprintf(stderr, "\nERROR : Binary code not found in table\n"); exit(EXIT_FAILURE);
            }
            filling++;
            if(filling==8){ //The buffer is filled, we can insert it in bufferOut
                bufferOut.text[bufferOut.size]=buffer;
                bufferOut.size++;
                buffer=0;
                filling=0;

                if(bufferOut.size==BUFFER_SIZE){ // The buffer is full
                    fwrite(bufferOut.text, sizeof(unsigned char), bufferOut.size, fileOut);
                    bufferOut.size=0;
                }
            }

            l = l->next;
        }
        posIn++;
        posTable=0;

        if(progress+5 < 100*(((double)posIn)/bufferBW.size))
        {
            progress+=5; 
            printf("%d%%\n", progress); //Displays the progress of the current task
        }
    }

    if(filling!=0){
        while(filling<8 && buffer!=0){
            buffer <<= 1;
            filling++;
        }
        bufferOut.text[bufferOut.size]=buffer;
        bufferOut.size++;
        buffer=0;
        filling=0;
    }
    if(bufferOut.size>0){ // The buffer isn't empty
        fwrite(bufferOut.text, sizeof(unsigned char), bufferOut.size, fileOut);
        bufferOut.size=0;
    }
    free(bufferOut.text);
}


/**
 * \fn void compressMain(char* fileNameIn)
 * \brief Main function for compression : calls required functions to the decompression of the file whose name is given to the function
 * \param fileNameIn Name of the file that is being compressed
 */

void compressMain(char* fileNameIn)
{
    FILE* fileIn;
    FILE* fileOut;
    long sizeFileIn;
    long sizeFileOut;
    int sizeHuffmanTable=0;
    int indexBW=-1; // if the value is still -1 when we write it in the table then we won't apply BW and MTF to decompress the file

    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    sizeFileIn = seekSizeOfFile(fileIn);

    FileBuffer bufferText=fileToBuffer(fileIn);
    FCLOSE(fileIn);



    if(sizeFileIn<1000)
    {
        //BURROWS WHEELER
        printf("\nBurrows Wheeler...\n");
        indexBW = burrowsWheeler(&bufferText);

        // MTF
        printf("\nMove To Front...\n");
        moveToFrontEncode(&bufferText);
    } 

    //TABLE CREATION
    printf("\nTable creation...\n");
    HuffmanTableCell* huffmanTable = createHuffmanTable(indexBW, bufferText, &sizeHuffmanTable);

    //COMPRESSION
    fileOut = fopen(strcat(fileNameIn, ".bin"), "wb+"); // We add a .bin at the end of the name so that the initial file isn't replaced
    TESTFOPEN(fileOut);
    printf("\nCompression...\n");

    compress(bufferText, fileOut, huffmanTable, sizeHuffmanTable);
    printf("\nEnd of compression\n");
    
    free(bufferText.text);
    freeHuffmanTable(huffmanTable, sizeHuffmanTable);
    sizeFileOut = seekSizeOfFile(fileOut);
    FCLOSE(fileOut);
    printf("\nSpace saving : %.2f %%\n\n", (1-(((float)sizeFileOut)/sizeFileIn))*100);
}
