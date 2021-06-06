/**
 * \file HuffmanFunctions.h
 * \brief Contains the functions prototypes
 * \author Robin Meneust
 * \date 2021
 */

#ifndef HuffmanFunctions
#define HuffmanFunctions
#include "Structures_Define.h"


//GlobalFunctions.c
void getFileName(char * fileName);
void bufferToFile(FileBuffer buffer, FILE* file);
FileBuffer fileToBuffer(FILE* file);
long readNumberLine(FILE* file, long line);
void wordWrapFile(FILE* file);
void wordWrapBuffer(FileBuffer buffer, int* posIn);
int seekSizeOfFile(FILE* file);
long seekNbFirstLineBuffer(FileBuffer buffer);

//HuffmanTableCreation.c
OccurrencesArrayCell* fillOccurrencesArray(FileBuffer buffer, int* sizeOccurrencesArray);
void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray);
void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray, int* sizeOccurrencesArray);
HuffmanTreeNode* createNodeHuff(unsigned char c, HuffmanTreeNode* leftNode, HuffmanTreeNode* rightNode, HuffmanTreeNode* parentNode);
void fillHuffmanTree(OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2);
void freeHuffmanTree(HuffmanTreeNode* huffmanNode);
void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray);
void readNodeHuffmanAndWrite(FILE* file, unsigned char * bufferChar, HuffmanTreePtr huffmanNode, int fileSize, int* pos, uint8_t *bufferPos, int* filling ,int* sizeBufferPosTotal);
FileBuffer saveTable(int indexBW, HuffmanTableCell* huffmanTable, int sizeHuffmanTable, int fileSize);
FileBuffer createHuffmanTable(int indexBW, FileBuffer bufferIn);


//Compression.c
void compress(FileBuffer bufferBW, FILE* fileOut, FileBuffer bufferTable);
void compressMain(char* fileNameIn);

//Decompression.c
int initializePossibleElementsArray(int* possibleElementsArray, FILE* table);
unsigned char seekFirstPositiveIndex(int* possibleElementsArray);
void refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements);
void decompress(FILE* fileIn, FileBuffer* bufferOut, HuffmanTreePtr huffmanTreeHead, int sizeFileIn);
void decompressMain();

//BurrowsWheeler.c
unsigned char ** allocateMatBW(long size);
void freeMatBW(unsigned char** matBW, long size);
void shiftedFilling(FILE* fileIn, unsigned char** matBW, long size, long position);
long sortMat(unsigned char** matBW, long size);
void saveBWEncode(FILE* fileBW, unsigned char** matBW, long size, long i0);
void fillColumnBWDecode(FileBuffer bufferBW, unsigned char** matBW, long size, long shift);
void saveBWDecode(FILE* fileBWDecode, unsigned char** matBW, long size, long i0);
void rotationSort(unsigned char* tabChar,int* indexes, int size);
int burrowsWheeler(FileBuffer* bufferIn);
void burrowsWheelerDecode(int indexBW, FileBuffer bufferIn, FILE* fileBWDecode);

//MoveToFront.c
int seekChar(unsigned char array[], int size, unsigned char c);
void shiftCharStart(unsigned char array[], int size, int index);
void moveToFrontEncode(FileBuffer *buffer);
void moveToFrontDecode(FileBuffer *buffer);

#endif
