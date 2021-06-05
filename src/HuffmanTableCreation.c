/**
 * \file HuffmanTableCreation.c
 * \brief Used to create the Huffman table that will be saved in a file used to compress and decompress the associated file
 * \author Robin Meneust
 * \date 2021
 */


#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"



/**
 * \fn OccurrencesArrayCell* fillOccurrencesArray(FILE* file, int* sizeOccurrencesArray)
 * \brief Allocates and initializes occurrencesArray that associate each character to its number of occurrences
 * \param buffer Buffer used to fill the array occurrencesArray
 * \param sizeOccurrencesArray Size of the array occurrencesArray(array_return here), it's incremented while the array is created
 * \return Array of structures containing each character associated to its number of occurrences in the initial file
 */


OccurrencesArrayCell* fillOccurrencesArray(FileBuffer buffer, int* sizeOccurrencesArray)
{
    int car[N_ASCII]={0};
    unsigned char c;
    *sizeOccurrencesArray=0;
    int pos=0;
    while(pos<buffer.size)
    {
        c=buffer.text[pos];
        if(car[c]==0)   // Used to increment the size of the return array, since it counts the number of unique elements in the buffer
            (*sizeOccurrencesArray)++;
        car[c]++;
        pos++;
    }

    OccurrencesArrayCell* array_return = (OccurrencesArrayCell*) malloc((*sizeOccurrencesArray) * sizeof(OccurrencesArrayCell));
    TESTALLOC(array_return);

    int i_return=0;
    for(int i=0; i<N_ASCII; i++)
    {
        if(car[i]!=0)
        {
            array_return[i_return].c = (unsigned char*) malloc((*sizeOccurrencesArray) * sizeof(unsigned char));
            TESTALLOC(array_return[i_return].c);
            array_return[i_return].size=1;
            array_return[i_return].c[0]=i;
            array_return[i_return].occurrences=car[i];
            array_return[i_return].mergedHead=NULL;
            i_return++;
        }
    }
    return array_return;
}

/**
 * \fn void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray)
 * \brief Seeks the 2 elements of occurrencesArray with the field occurrences with the 2 lowest values (the rarest ones)
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param sizeOccurrencesArray Size of the array occurrencesArray
 */


void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray)
{
    for(int i=2; i<sizeOccurrencesArray; i++){
        if(occurrencesArray[i].occurrences < occurrencesArray[*i_min2].occurrences){
            *i_min2 = i;
            if (occurrencesArray[*i_min2].occurrences < occurrencesArray[*i_min1].occurrences){
                int tmp=*i_min2;
                *i_min2 = *i_min1;
                *i_min1 = tmp;
            }
        }
    }
}

/**
 * \fn void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray, int* sizeOccurrencesArray)
 * \brief Merges 2 cells of occurrencesArray, shift the rest of the array and switch the cell i_min2 with the last cell of the array and reduce the size of the array
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param sizeOccurrencesArray Size of the array occurrencesArray
 */

void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray, int* sizeOccurrencesArray)
{
    for(int i=occurrencesArray[i_min1].size; i<occurrencesArray[i_min1].size+occurrencesArray[i_min2].size; i++) // Merges the strings
    {
        occurrencesArray[i_min1].c[i]=occurrencesArray[i_min2].c[i-occurrencesArray[i_min1].size];
    }
    occurrencesArray[i_min1].size+=occurrencesArray[i_min2].size;
    occurrencesArray[i_min1].occurrences+= occurrencesArray[i_min2].occurrences;

    occurrencesArray[i_min2]=occurrencesArray[(*sizeOccurrencesArray)-1];
    (*sizeOccurrencesArray)--;
}


HuffmanTreeNode* createNodeHuff(unsigned char c, HuffmanTreeNode* leftNode, HuffmanTreeNode* rightNode, HuffmanTreeNode* parentNode)
{
    HuffmanTreeNode* new_node = (HuffmanTreeNode*) malloc(sizeof(HuffmanTreeNode));
    TESTALLOC(new_node);
    new_node->c=c;
    new_node->left=leftNode;
    new_node->right=rightNode;
    new_node->parent=parentNode;
    return new_node;
}



/**
 * \fn void fillHuffmanTree(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
 * \brief Fills the cells of huffmanTable that correpond to the characters contained in the field c of tabOcurrences for the cells i_min1 and i_min2 with 0 or 1
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of the array huffmanTable
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 */


void fillHuffmanTree(OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
{
    HuffmanTreeNode * leftNode=NULL;
    HuffmanTreeNode * rightNode=NULL;
    HuffmanTreeNode * mergedNode=NULL;

    if(occurrencesArray[i_min1].mergedHead==NULL){  // In this case it's a node that hasn't been merged with another one yet so we have to create it
        leftNode = createNodeHuff(occurrencesArray[i_min1].c[0], NULL, NULL, NULL);
    }
    else{
        leftNode = occurrencesArray[i_min1].mergedHead;
    }
    if(occurrencesArray[i_min2].mergedHead==NULL){
        rightNode = createNodeHuff(occurrencesArray[i_min2].c[0], NULL, NULL, NULL);
    }
    else{
        rightNode = occurrencesArray[i_min2].mergedHead;
    }

    mergedNode=createNodeHuff('\0', leftNode, rightNode, NULL);
    leftNode->parent= mergedNode;
    rightNode->parent= mergedNode;
    occurrencesArray[i_min1].mergedHead = mergedNode;
}


/**
 * \fn void freeHuffmanTree(HuffmanTreeNode* huffmanTable)
 * \brief Frees the memory used by huffmanTable and its linked lists (of the field code)
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of huffmanTable
 */


void freeHuffmanTree(HuffmanTreeNode* huffmanNode)
{
    if(huffmanNode==NULL){
        return;
    }
    else{
        freeHuffmanTree(huffmanNode->left);
        freeHuffmanTree(huffmanNode->right);
        free(huffmanNode);
    }
}

/**
 * \fn void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray)
 * \brief Frees the memory used by occurrencesArray and its dynamic arrays (of the field c)
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param sizeOccurrencesArray Size of occurrencesArray
 */


void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray, int sizeOccurrencesArray)
{
    for(int i=0; i<sizeOccurrencesArray; i++)
        free(occurrencesArray[i].c);
    free(occurrencesArray);
}



void readNodeHuffmanAndWrite(FILE* file, unsigned char * bufferChar, HuffmanTreeHead huffmanNode, int fileSize, int* pos)
{
    if(huffmanNode==NULL){
        fputc('0', file);
        printf("0");
        return;
    }
    else{
        if(*pos==fileSize){
            fprintf(stderr, "\nERROR : fulled buffer in the huffman table creation\n");
            exit(EXIT_FAILURE);
        }
        fputc('1', file);
        printf("1");
        if(huffmanNode->left==NULL && huffmanNode->right==NULL){
            bufferChar[*pos] = huffmanNode->c;
            printf("\nc : %c|%d\n", bufferChar[*pos], bufferChar[*pos]);
            (*pos)++;
        }
        readNodeHuffmanAndWrite(file, bufferChar, huffmanNode->left, fileSize, pos);
        readNodeHuffmanAndWrite(file, bufferChar, huffmanNode->right, fileSize, pos);
    }
}

/**
 * \fn void saveTable(int indexBW, HuffmanTableCell* huffmanTable, int sizeHuffmanTable, int fileSize)
 * \brief Saves the Huffman coding table in a text file
 * \param indexBW Index used to know if BW was used (if >=0) and to decode a file on which Burrows Wheeler was used
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of huffmanTable
 * \param fileSize Number of characters in the initial uncompressed file
 */


void saveTable(int indexBW, HuffmanTreeHead huffmanTable, int fileSize)
{
    int pos=0; // Used to write in the buffer bufferChar
    FILE* fileTable = fopen("table.txt", "wb+");
    TESTFOPEN(fileTable);
    fprintf(fileTable, "%d\n%d\n", indexBW, fileSize);

    unsigned char* bufferChar = (unsigned char*) malloc(fileSize*sizeof(unsigned char));
    TESTALLOC(bufferChar);
    readNodeHuffmanAndWrite(fileTable, bufferChar, huffmanTable, fileSize, &pos);
    fputc('\n', fileTable);
    fwrite(bufferChar, sizeof(unsigned char), fileSize, fileTable);

    FCLOSE(fileTable);
}


/**
 * \fn void createHuffmanTable(int indexBW, FileBuffer bufferIn)
 * \brief Creates, fills and saves huffmanTable that associate each character of bufferIn to a binary code
 * \param indexBW Index used to know if BW was used (if >=0) and to decode a file on which Burrows Wheeler was used
 * \param bufferIn Buffer from which we create the table associated
 */

void createHuffmanTable(int indexBW, FileBuffer bufferIn)
{
    int sizeOccurrencesArray;
    OccurrencesArrayCell* occurrencesArray = fillOccurrencesArray(bufferIn, &sizeOccurrencesArray);
    int nbUniqueElements=sizeOccurrencesArray;
    int i_min1;
    int i_min2;

    while(sizeOccurrencesArray>1){  // Until there is only one element left
        i_min1 = 0;
        i_min2 = 1;
        if (occurrencesArray[i_min2].occurrences < occurrencesArray[i_min1].occurrences){
            int tmp=i_min2;
            i_min2 = i_min1;
            i_min1 = tmp;
        }
        seek2Min(&i_min1, &i_min2, occurrencesArray,sizeOccurrencesArray);
        fillHuffmanTree(occurrencesArray, i_min1, i_min2);
        merge(i_min1, i_min2, occurrencesArray, &sizeOccurrencesArray);
    }

    saveTable(indexBW, occurrencesArray[i_min1].mergedHead, nbUniqueElements);
    freeHuffmanTree(occurrencesArray[i_min1].mergedHead);
    freeOccurrencesArray(occurrencesArray, sizeOccurrencesArray);
}
