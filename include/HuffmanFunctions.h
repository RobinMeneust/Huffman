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
FileBuffer getPortionOfFileToBuffer(FILE* file, int sizeBuff);
FileBuffer fileToBuffer(FILE* file);
long readNumberLine(FILE* file, long line);
void wordWrapBuffer(FileBuffer buffer, int* posIn);
void wordWrapFile(FILE* file);
int seekSizeOfFile(FILE* file);
HuffmanTreeNode* createNodeHuff(unsigned char c, HuffmanTreeNode* leftNode, HuffmanTreeNode* rightNode, HuffmanTreeNode* parentNode);



//HuffmanTableCreation.c
OccurrencesArrayCell* fillOccurrencesArray(FileBuffer buffer, int* sizeOccurrencesArray);
PtrlistCode createNode(unsigned char c);
void addStartList(PtrlistCode *liste, PtrlistCode node);
void initializeCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray);
OccurrencesArrayCell* fillOccurrencesArray(FileBuffer buffer, int* sizeOccurrencesArray);
void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray);
void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray, int* sizeOccurrencesArray);
void fillHuffmanTableCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2);
void freeHuffmanTable(HuffmanTableCell* huffmanTable, int sizeHuffmanTable);
void fillHuffmanTree(OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2);
void freeHuffmanTree(HuffmanTreeNode* huffmanNode);
void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray);
void readNodeHuffmanAndWrite(FileBuffer* bufferChar, FileBuffer* bufferPos, HuffmanTreePtr huffmanNode, int* posBufferChar, int* posBufferPos, uint8_t *buffer, int* filling);
void saveTree(int indexBW, HuffmanTreePtr huffmanTree, int sizeBufferChar, int fileSize);
FileBuffer saveTable(HuffmanTableCell* huffmanTable, int sizeHuffmanTable);
FileBuffer createHuffmanTable(int indexBW, FileBuffer bufferIn);

//Compression.c
void compress(FileBuffer bufferBW, FILE* fileOut, FileBuffer bufferTable);
void compressMain(char* fileNameIn);

//Decompression.c
HuffmanTreePtr createTreeFromBuffers(FileBuffer bufferPos, FileBuffer bufferChar);
int readTreeFromPos(HuffmanTreePtr* huffmanTreePos, uint8_t bit);
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
