/**
 * \file HuffmanTableCreation.c
 * \brief Used to create the Huffman table that will be saved in a file used to compress and decompress the associated file
 * \author Robin Meneust
 * \date 2021
 */


#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"


/**
 * \fn PtrlistCode createNode(unsigned char c)
 * \brief Creates a new node for a listCode type linked list
 * \param c Character contained in the newly created node
 * \return Created node
 */

PtrlistCode createNode(unsigned char c)
{
    listCode *node = NULL;
    node= (listCode*) malloc(sizeof(listCode));
    TESTALLOC(node);
    node->value = c;
    node->next = NULL;
    return node;
}


/**
 * \fn void addStartList(PtrlistCode *liste, PtrlistCode node)
 * \brief Adds a node at the beginning of a given listCode type linked list
 * \param liste Pointer to the head of the give nlist
 * \param node Inserted node
 */

void addStartList(PtrlistCode *liste, PtrlistCode node)
{
    if(*liste!=NULL){
        node->next = *liste;
        *liste = node;
    }
    else{
        *liste=node;
    }
}


/**
 * \fn void initializeCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray)
 * \brief Initializes all the lists of the field code of a HuffmanTableCell type array
 * \param huffmanTable Array that is being initialized
 * \param sizeHuffmanTable Size of huffmanTable
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 */


void initializeCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray)
{
    for(int i=0; i<sizeHuffmanTable; i++){
        huffmanTable[i].code=NULL;
        huffmanTable[i].c=occurrencesArray[i].c[0];
    }
}


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


/**
 * \fn void fillHuffmanTableCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
 * \brief Fills the cells of huffmanTable that correpond to the characters contained in the field c of tabOcurrences for the cells i_min1 and i_min2 with 0 or 1
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of the array huffmanTable
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 */


void fillHuffmanTableCode(HuffmanTableCell* huffmanTable, int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
{
    int j;
    for(int i=0; i<occurrencesArray[i_min1].size; i++){
        j=0;
        while(occurrencesArray[i_min1].c[i]!=huffmanTable[j].c && j<sizeHuffmanTable){
            j++;
        }
        if(occurrencesArray[i_min1].c[i]==huffmanTable[j].c)
            addStartList(&(huffmanTable[j].code), createNode('0'));
    }

    for(int i=0; i<occurrencesArray[i_min2].size; i++){
        j=0;
        while(occurrencesArray[i_min2].c[i]!=huffmanTable[j].c && j<sizeHuffmanTable){
            j++;
        }
        if(occurrencesArray[i_min2].c[i]==huffmanTable[j].c)
            addStartList(&(huffmanTable[j].code), createNode('1'));
    }
}



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



/**
 * \fn void saveTable(int indexBW, HuffmanTableCell* huffmanTable, int sizeHuffmanTable, int fileSize)
 * \brief Saves the Huffman coding table in a text file
 * \param indexBW Index used to know if BW was used (if >=0) and to decode a file on which Burrows Wheeler was used
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of huffmanTable
 * \param fileSize Number of characters in the initial uncompressed file
 */


void saveTable(int indexBW, HuffmanTableCell* huffmanTable, int sizeHuffmanTable, int fileSize)
{
    FILE* fileTable = fopen("table.txt", "wb+");
    TESTFOPEN(fileTable);
    PtrlistCode l=NULL;
    fprintf(fileTable, "%d\n%d\n", indexBW, fileSize);
    printf("INDEX %d\nFILESIZE %d\n", indexBW, fileSize);
    for(int i=0; i<sizeHuffmanTable; i++){
        l = huffmanTable[i].code;
        fputc(huffmanTable[i].c, fileTable);
        while(l!=NULL){
            fputc(l->value, fileTable);
            l = l->next;
        }
        fputc('\n', fileTable);
    }
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

    int sizeHuffmanTable=sizeOccurrencesArray;
    HuffmanTableCell* huffmanTable = NULL;
    huffmanTable = (HuffmanTableCell*) malloc(sizeHuffmanTable*sizeof(HuffmanTableCell));
    TESTALLOC(huffmanTable);
    initializeCode(huffmanTable, sizeHuffmanTable, occurrencesArray);

    while(sizeOccurrencesArray>1){  // Until there is only one element left
        int i_min1 = 0;
        int i_min2 = 1;
        if (occurrencesArray[i_min2].occurrences < occurrencesArray[i_min1].occurrences){
            int tmp=i_min2;
            i_min2 = i_min1;
            i_min1 = tmp;
        }
        seek2Min(&i_min1, &i_min2, occurrencesArray,sizeOccurrencesArray);
        fillHuffmanTableCode(huffmanTable, sizeHuffmanTable, occurrencesArray, i_min1, i_min2);
        merge(i_min1, i_min2, occurrencesArray, &sizeOccurrencesArray);
    }

    saveTable(indexBW, huffmanTable, sizeHuffmanTable, bufferIn.size);

    freeHuffmanTable(huffmanTable, sizeHuffmanTable);
    freeOccurrencesArray(occurrencesArray, sizeOccurrencesArray);
}
