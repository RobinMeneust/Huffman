/**
 * \file HuffmanTableCreation.c
 * \brief Used to create the Huffman table that will be saved in a file used to compress and decompress the associated file
 * \author Robin Meneust
 * \date 2021
*/


#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


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
 * \fn void initializeCode(HuffmanTableCell* huffmanTable,int sizeHuffmanTable,OccurrencesArrayCell* occurrencesArray)
 * \brief Initializes all the lists of the field code of a HuffmanTableCell type array
 * \param huffmanTable Array that is being initialized
 * \param sizeHuffmanTable Size of the dynamic array huffmanTable
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
*/


void initializeCode(HuffmanTableCell* huffmanTable,int sizeHuffmanTable,OccurrencesArrayCell* occurrencesArray)
{
    for(int i=0; i<sizeHuffmanTable; i++){
        huffmanTable[i].code=NULL;
        huffmanTable[i].c=occurrencesArray[i].c[0];
    }
}


/**
 * \fn OccurrencesArrayCell* rempliroccurrencesArray(FILE* file,int* occurrencesArraySize)
 * \brief Allocates and initializes occurrencesArray that associate each character to its number of occurrences
 * \param buffer Buffer used to fill the array occurrencesArray
 * \param occurrencesArraySize Size of the dynamic array occurrencesArray(t_return here), it's incremented while the array is created
 * \return Array of structures containing each character associated to its number of occurrences in the initial file
*/


OccurrencesArrayCell* rempliroccurrencesArray(FileBuffer buffer,int* occurrencesArraySize)
{
    int car[N_ASCII]={0};
    unsigned char c;
    *occurrencesArraySize=0;
    OccurrencesArrayCell* t_return = NULL;
    int pos=0;
    while(pos<buffer.size)
    {
        c=buffer.text[pos];
        car[c]++;
        pos++;
    }

    int idcar=0;
    for(int i=0;i<N_ASCII;i++)
    {
        if(car[i]!=0)
        {
            (*occurrencesArraySize)++;
            t_return=(OccurrencesArrayCell*)realloc(t_return,sizeof(OccurrencesArrayCell)*(*occurrencesArraySize));
            TESTALLOC(t_return);
            t_return[idcar].c = (unsigned char*) malloc(sizeof(unsigned char));
            TESTALLOC(t_return[idcar].c);
            t_return[idcar].size=1;
            t_return[idcar].c[0]=i;
            t_return[idcar].occurrences=car[i];
            idcar++;
        }
    }
    return t_return;
}

/**
 * \fn void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray,int occurrencesArraySize)
 * \brief Seeks the 2 elements of occurrencesArray with the field occurrences with the 2 lowest values (the rarest ones)
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param occurrencesArraySize Size of the dynamic array occurrencesArray
*/


void seek2Min(int* i_min1, int* i_min2, OccurrencesArrayCell* occurrencesArray,int occurrencesArraySize)
{
    for(int i=2; i<occurrencesArraySize; i++){
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
 * \fn void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray,int* occurrencesArraySize)
 * \brief Merges 2 cells of occurrencesArray, shift the rest of the array and switch the cell i_min2 with the last cell of the array and reallocate to reduce the size of the array
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param occurrencesArraySize Size of the dynamic array occurrencesArray
*/

void merge(int i_min1, int i_min2, OccurrencesArrayCell* occurrencesArray,int* occurrencesArraySize)
{
    occurrencesArray[i_min1].c=(unsigned char*)realloc(occurrencesArray[i_min1].c,sizeof(unsigned char)*(occurrencesArray[i_min1].size+occurrencesArray[i_min2].size));
    TESTALLOC(occurrencesArray[i_min1].c);
    for(int i=occurrencesArray[i_min1].size;i<occurrencesArray[i_min1].size+occurrencesArray[i_min2].size;i++)
    {
        occurrencesArray[i_min1].c[i]=occurrencesArray[i_min2].c[i-occurrencesArray[i_min1].size];
    }
    occurrencesArray[i_min1].size+=occurrencesArray[i_min2].size;

    occurrencesArray[i_min1].occurrences+= occurrencesArray[i_min2].occurrences;


    occurrencesArray[i_min2]=occurrencesArray[(*occurrencesArraySize)-1];
    (*occurrencesArraySize)--;
    occurrencesArray=(OccurrencesArrayCell*)realloc(occurrencesArray,*occurrencesArraySize*sizeof(OccurrencesArrayCell));
    TESTALLOC(occurrencesArray);
}


/**
 * \fn void fillHuffmanTableCode(HuffmanTableCell* huffmanTable,int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
 * \brief Fills the cells of huffmanTable that correpond to the characters contained in the field c of tabOcurrences for the cells i_min1 and i_min2 with 0 or 1
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of the dynamic array huffmanTable
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param i_min1 Pointer to the index of the first minimum of the field occurrences
 * \param i_min2 Pointer to the index of the second minimum of the field occurrences
*/


void fillHuffmanTableCode(HuffmanTableCell* huffmanTable,int sizeHuffmanTable, OccurrencesArrayCell* occurrencesArray, int i_min1, int i_min2)
{
    int j;
    for(int i=0;i<occurrencesArray[i_min1].size;i++){
        j=0;
        while(occurrencesArray[i_min1].c[i]!=huffmanTable[j].c && j<sizeHuffmanTable){
            j++;
        }
        if(occurrencesArray[i_min1].c[i]==huffmanTable[j].c)
            addStartList(&(huffmanTable[j].code), createNode('0'));
    }

    for(int i=0;i<occurrencesArray[i_min2].size;i++){
        j=0;
        while(occurrencesArray[i_min2].c[i]!=huffmanTable[j].c && j<sizeHuffmanTable){
            j++;
        }
        if(occurrencesArray[i_min2].c[i]==huffmanTable[j].c)
            addStartList(&(huffmanTable[j].code), createNode('1'));
    }
}



/**
 * \fn void freeHuffmanTable(HuffmanTableCell* huffmanTable,int sizeHuffmanTable)
 * \brief Frees the memory used by huffmanTable and its linked lists (of the field code)
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of the dynamic array huffmanTable
*/


void freeHuffmanTable(HuffmanTableCell* huffmanTable,int sizeHuffmanTable)
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
 * \fn void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray,int occurrencesArraySize)
 * \brief Frees the memory used by occurrencesArray and its dynamic arrays (of the field c)
 * \param occurrencesArray Array of structures containing each character associated to its number of occurrences in the initial file
 * \param occurrencesArraySize size du tableau dynamique occurrencesArray
*/


void freeOccurrencesArray(OccurrencesArrayCell* occurrencesArray,int occurrencesArraySize)
{
    for(int i=0; i<occurrencesArraySize; i++)
        free(occurrencesArray[i].c);
    free(occurrencesArray);
}



/**
 * \fn void saveTable(int indexBW HuffmanTableCell* huffmanTable,int sizeHuffmanTable, int fileSize, unsigned char option)
 * \brief Saves the Huffman coding table in a text file
 * \param indexBW Index used to decode a file on which Burrows Wheeler was used
 * \param huffmanTable Array of structures HuffmanTableCell containing all the characters associated to a sequence of 0 or 1 depending of their number of occurrences in the initial file
 * \param sizeHuffmanTable Size of the dynamic array huffmanTable
 * \param fileSize Number of characters in the initial uncompressed file
 * \param option Contains 0 if we should not apply BW and MTF or 1 if we should.
*/


void saveTable(int indexBW, HuffmanTableCell* huffmanTable,int sizeHuffmanTable, int fileSize, unsigned char option)
{
    FILE* fileTable = fopen("table.txt", "wb+");
    TESTFOPEN(fileTable);
    PtrlistCode l=NULL;
    fprintf(fileTable, "%d\n%c\n%d\n", indexBW, option, fileSize);
    printf("DEB\n\n%c\n%d\n\n", option, fileSize);
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
 * \fn void createHuffmanTable(int indexBW, FileBuffer bufferIn, unsigned char option)
 * \brief Creates, fills and saves huffmanTable that associate each character of bufferIn to a binary code
 * \param indexBW Index used to decode a file on which Burrows Wheeler was used
 * \param bufferIn Buffer from which we create the table associated
 * \param option Contains 0 if we should not apply BW and MTF or 1 if we should.
*/

void createHuffmanTable(int indexBW, FileBuffer bufferIn, unsigned char option)
{
    int occurrencesArraySize;
    OccurrencesArrayCell* occurrencesArray = rempliroccurrencesArray(bufferIn,&occurrencesArraySize);

    int sizeHuffmanTable=occurrencesArraySize;
    HuffmanTableCell* huffmanTable = NULL;
    huffmanTable = (HuffmanTableCell*) malloc(sizeHuffmanTable*sizeof(HuffmanTableCell));
    TESTALLOC(huffmanTable);
    initializeCode(huffmanTable,sizeHuffmanTable,occurrencesArray);

    while(occurrencesArraySize>1){  // Until there is only one element left
        int i_min1 = 0;
        int i_min2 = 1;
        if (occurrencesArray[i_min2].occurrences < occurrencesArray[i_min1].occurrences){
            int tmp=i_min2;
            i_min2 = i_min1;
            i_min1 = tmp;
        }
        seek2Min(&i_min1, &i_min2, occurrencesArray,occurrencesArraySize);
        fillHuffmanTableCode(huffmanTable, sizeHuffmanTable, occurrencesArray, i_min1, i_min2);
        merge(i_min1, i_min2, occurrencesArray,&occurrencesArraySize);
    }

    saveTable(indexBW, huffmanTable, sizeHuffmanTable, bufferIn.size, option);

    freeHuffmanTable(huffmanTable,sizeHuffmanTable);
    freeOccurrencesArray(occurrencesArray,occurrencesArraySize);
}
