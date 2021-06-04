/**
 * \file Compression.c
 * \brief Compress the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn void compress(FileBuffer bufferBW, FILE* fileOut)
 * \brief Compress bufferBW by using the Huffman table
 * \param bufferBW Buffer that is being compressed
 * \param fileOut File compressed filled in this function by using the Huffman coding
*/

void compress(FileBuffer bufferBW, FILE* fileOut)
{
    uint8_t buffer=0;   // Byte used to contain the binary code before being inserted in the file fileOut
    int filling=0;
    int progress=0;
    unsigned char c, c_table;
    FILE* table = fopen("table.txt", "rb");
    TESTFOPEN(table);
    rewind(fileOut);
    int pos=0;
    while(pos<bufferBW.size){
        c = bufferBW.text[pos];
        rewind(table);
        wordWrapFile(table);
        wordWrapFile(table);
        wordWrapFile(table);
        c_table = fgetc(table);
        while(c_table!=c && !feof(table)){
            wordWrapFile(table);
            c_table = fgetc(table);
        }
        if(c_table!=c){
                printf("ERROR : Character not found in the table : %c|%d",c,c);
            exit(EXIT_FAILURE);
        }

        c_table = fgetc(table);
        while(c_table!='\n' && !feof(table)){
            switch(c_table){
                case '0': buffer <<= 1; break;  //We shift bits to the left, then we add the bit (0 here) to the right
                case '1': buffer <<= 1; buffer|=0b1; break; //We shift bits to the left, then we add the bit (1 here) to the right (ici 1), we use a mask to do so
            }
            filling++;
            if(filling==8){ //The buffer is filled, we can insert it in the file
                putc(buffer, fileOut);
                buffer=0;
                filling=0;
            }
            c_table = fgetc(table);
        }
        pos++;

        if(progress+5<((pos+1)*100)/bufferBW.size)
        {
            progress=(int)((((pos+1)*100)/bufferBW.size)/5)*5; //Display the progress of the current task
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
    FCLOSE(table);
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
    int indexBW=-1;

    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    sizeFileIn = seekSizeOfFile(fileIn);
    unsigned char option=0;
    if(sizeFileIn<1000)
    {
        option=1;
    }

    FileBuffer buffertext=fileToBuffer(fileIn);
    FCLOSE(fileIn);

    if(option==1)
    {
        //BURROWS WHEELER
        printf("\nBurrows Wheeler...\n");
        indexBW = burrowsWheeler(&buffertext);
        
        // MTF
        printf("\nMove To Front...\n");
        moveToFrontEncode(&buffertext);
    } 

    //TABLE CREATION
    createHuffmanTable(indexBW, buffertext, option+'0');

    //COMPRESSION
    fileOut = fopen(strncat(fileNameIn, ".bin", 4), "wb+"); // We add a .bin at the end of the name so that the initial file isn't replaced
    TESTFOPEN(fileOut);
    printf("\nCompression...\n");
    compress(buffertext, fileOut);
    printf("\nEnd of compression\n");
    FCLOSE(fileOut);
    free(buffertext.text);
}
