/**
 * \file Compression.c
 * \brief Compresses the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
 */

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"


/**
 * \fn void compress(FileBuffer bufferBW, FILE* fileOut)
 * \brief Compresses bufferBW by using the Huffman table
 * \param bufferBW Buffer that is being compressed
 * \param fileOut File compressed filled in this function by using the Huffman coding
  */

void compress(FileBuffer bufferBW, FILE* fileOut, FileBuffer bufferTable)
{
    uint8_t buffer=0;   // Byte used to contain the binary code before being inserted in the file fileOut
    int filling=0;
    int progress=0;
    unsigned char c, c_table;
    rewind(fileOut);
    int posIn=0;
    int posTable=0;
    while(posIn<bufferBW.size){
        c = bufferBW.text[posIn];
        c_table = bufferTable.text[posTable];
        posTable++;
        while(c_table!=c && posTable<bufferTable.size){
            wordWrapBuffer(bufferTable, &posTable);
            c_table = bufferTable.text[posTable];
            posTable++;
        }

        if(c_table!=c){
            printf("ERROR : Character not found in the table : %c|%d", c, c);
            exit(EXIT_FAILURE);
        }

        c_table = bufferTable.text[posTable];
        posTable++;
        while(c_table!='\n' && posTable<bufferTable.size){
            switch(c_table){
                case '0': buffer <<= 1; break;  //We shift bits to the left, then we add the bit (0 here) to the right
                case '1': buffer <<= 1; buffer|=0b1; break; //We shift bits to the left, then we add the bit (1 here) to the right, we use a mask to do so
                default : fprintf(stderr, "\nERROR : Binary code not found in table\n"); exit(EXIT_FAILURE);
            }
            filling++;
            if(filling==8){ //The buffer is filled, we can insert it in the file
                putc(buffer, fileOut);
                buffer=0;
                filling=0;
            }
            c_table = bufferTable.text[posTable];
            posTable++;
        }
        posIn++;
        posTable=0;

        if(progress+5<((posIn+1)*100)/bufferBW.size)
        {
            progress=(int)((((posIn+1)*100)/bufferBW.size)/5)*5; //Display the progress of the current task
            printf("%d%%\n", progress);
        }

    }
    if(filling!=0){
        while(filling<8 && buffer!=0){
            buffer <<= 1;
            filling++;
        }
        putc(buffer, fileOut);
        buffer=0;
        filling=0;
    }
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
    FileBuffer bufferTable = createHuffmanTable(indexBW, bufferText);


    //COMPRESSION
    fileOut = fopen(strncat(fileNameIn, ".bin", 4), "wb+"); // We add a .bin at the end of the name so that the initial file isn't replaced
    TESTFOPEN(fileOut);
    printf("\nCompression...\n");


    compress(bufferText, fileOut, bufferTable);
    printf("\nEnd of compression\n");
    
    free(bufferText.text);
    free(bufferTable.text);
    sizeFileOut = seekSizeOfFile(fileOut);
    FCLOSE(fileOut);
    printf("\nSpace saving : %.2f %%\n\n", (1-(((float)sizeFileOut)/sizeFileIn))*100);
}
