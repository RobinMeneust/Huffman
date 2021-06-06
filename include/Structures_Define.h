/**
 * \file Structures_Define.h
 * \brief Contains library includes, constants, macros and project structures
 * \author Robin Meneust
 * \date 2021
 */


#ifndef StructuresDefine
#define StructuresDefine

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//For uint8_t in Compression.c and Decompression.c
#include <inttypes.h>

//For the access function used in DecompressionMain
#if __linux__
#include <unistd.h>
#endif
#if __WIN32__
#include <io.h>
#endif

//Constants definitions

/**
 * \def N_ASCII Constant correponding to the number of possible values taken by a byte
 */

#define N_ASCII 256

/**
 * \def FCLOSE(X) Macro used to check if a file was closed correctly, if not then the program is stopped
 */

#define FCLOSE(X){\
            if(fclose(X)==EOF){\
                fprintf(stderr, "\nERROR : Cannot close the file\n"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \def TESTFOPEN(X) MMacro used to check if a file was opened correctly, if not then the program is stopped
 */

#define TESTFOPEN(X){\
            if(X==NULL){\
                fprintf(stderr, "\nERROR : Cannot open the file\n"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \def TESTALLOC(X) Macro used to check if a pointer was correctly associated to an memory adress after a dynamic allocatione, if not then the program is stopped
 */

#define TESTALLOC(X){\
            if(X==NULL){\
                fprintf(stderr, "\nERROR : Cannot allocate memory\n"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \struct FileBuffer Structures_Define.h
 * \brief Buffer containing a file or text
 */

typedef struct FileBuffer{
    unsigned char* text; /*!< text contained in the buffer*/
    int size; /*!< size of the buffer (number of characters)*/
}FileBuffer;

/**
 * \struct listCode Structures_Define.h
 * \brief Node of a linked list containing a sequence of characters 0 or 1
 */

typedef struct listCode{
    unsigned char value; /*!< character contained in the node (0 ou 1)*/
    struct listCode *next; /*!< pointer to the next node*/
}listCode;

typedef listCode* PtrlistCode;


/**
 * \struct H Structures_Define.h
 * \brief 
 */

typedef struct HuffmanTreeNode{
    unsigned char c; /*!< character to which we associate the code*/
    struct HuffmanTreeNode* left;
    struct HuffmanTreeNode* right;
    struct HuffmanTreeNode* parent;
}HuffmanTreeNode;

typedef HuffmanTreeNode* HuffmanTreePtr;



/**
 * \struct HuffmanTableCell Structures_Define.h
 * \brief Cell of an array associating a character to a binary code
 */

typedef struct HuffmanTableCell{
    unsigned char c; /*!< character to which we associate the code*/
    PtrlistCode code; /*!< linked list containing the binary code associated to the character c*/
}HuffmanTableCell;




/**
 * \struct OccurrencesArrayCell Structures_Define.h
 * \brief Cell of an array associating characters to their number of occurrences and to the size of c : the string containing those characters
 */

typedef struct OccurrencesArrayCell{
    int occurrences; /*!< number of occurrences of the characters contained in c*/
    unsigned char* c; /*!< string*/
    int size; /*!< size of the string c*/
    HuffmanTreePtr mergedHead;    
}OccurrencesArrayCell;



#endif
