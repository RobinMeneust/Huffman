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
int seekStringSize(char * string);
void bufferToFile(FileBuffer buffer,FILE* file);
FileBuffer fileToBuffer(FILE* file);
long readNumberLine(FILE* file, long line);
void wordWrapFile(FILE* file);
int wordWrapBuffer(FileBuffer buffer);
int seekSizeOfFile(FILE* file);
long seekSizeInBuffer(FileBuffer buffer);

//HuffmanTableCreation.c
PtrlistCode createNode(unsigned char c);
void addStartList(PtrlistCode *liste, PtrlistCode node);
void initializeCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray);
OccurrencesArrayCell* rempliroccurrencesArray(FileBuffer buffer, int* occurrencesArraySize);
void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray, int occurrencesArraySize);
void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray, int* occurrencesArraySize);
void fillHuffmanTableCode(HuffmanTableCell* huffmanTable,int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2);
void freeHuffmanTable(HuffmanTableCell* huffmanTable, int sizeHuffmanTable);
void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray, int occurrencesArraySize);
void saveTable(int indexBW, HuffmanTableCell* huffmanTable, int sizeHuffmanTable, int fileSize, unsigned char option);
void createHuffmanTable(int indexBW, FileBuffer bufferIn, unsigned char option);

//Compression.c
void compress(FileBuffer bufferBW, FILE* fileOut);
void compressMain(char* fileNameIn);

//Decompression.c
int initializePossibleElementsArray(int* possibleElementsArray, FILE* table);
unsigned char seekFirstPositiveIndex(int* possibleElementsArray);
void refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements);
void decompress(FILE* fileIn, FileBuffer* bufferOut, FILE* fileTable);
void decompressMain();

//BurrowsWheeler.c
unsigned char ** allocateMatBW(long size);
void freeMatBW(unsigned char** matBW, long size);
void shiftedFilling(FILE* fileIn, unsigned char** matBW, long size, long position);
void sortMat(unsigned char** matBW, long size);
void saveBWEncode(FILE* fileBW, unsigned char** matBW, long size, long i0);
void fillColumnBWDecode(FileBuffer bufferBW, unsigned char** matBW, long size, long shift);
void saveBWDecode(FILE* fileBWDecode, unsigned char** matBW, long size, long i0);
void rotationSort(unsigned char* tabChar,int* indexes, int size);
int burrowsWheeler(FileBuffer* bufferIn);
void burrowsWheelerDecode(int indexBW, FileBuffer bufferIn, FILE* fileBWDecode);

//MoveToFront.c
int seekChar(unsigned char array[], int size, unsigned char c);
void shiftCharStart(unsigned char array[],int size, int index);
void moveToFrontEncode(FileBuffer *buffer);
void moveToFrontDecode(FileBuffer *buffer);

#endif
