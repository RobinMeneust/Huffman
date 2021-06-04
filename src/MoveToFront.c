/**
 * \file MoveToFront.c
 * \brief Applied Move To Front to the given file or applies its inverse
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int seekChar(unsigned char tab[], int size, unsigned char c)
 * \brief Seeks the index of the first occurrence of the seeked character
 * \param tab Array in which we seek the character
 * \param size Size of the array tab
 * \param c Seeked character
 * \return Index of the character seeked in tab
*/


int seekChar(unsigned char tab[], int size, unsigned char c)
{
    int i=0;

    while(i<size){
        if(tab[i]==c){
            return i;
        }
        i++;
    }
    fprintf(stderr, "ERROR : Cannot find the character in seekChar in the function MTF");
    exit(EXIT_FAILURE);
}


/**
 * \fn void shiftCharStart(unsigned char tab[], int size, int index)
 * \brief Moves to the beginning the character of the given index l'index and shift all the characters between the beginning and this index
 * \param tab Array in which we shift the character at the given index
 * \param size Size of the array tab
 * \param index Index of the character that has to be shifted
*/


void shiftCharStart(unsigned char tab[],int size, int index)
{
    int i=index;
    unsigned char temp=tab[index];
    while(i>0){
        tab[i] = tab[i-1];
        i--;
    }
    tab[0]=temp;
}

/**
 * \fn void moveToFrontEncode(FileBuffer *buffer)
 * \brief Applies Move to front to the given buffer
 * \param buffer Buffer on which is applied MTF
*/

void moveToFrontEncode(FileBuffer *buffer)
{
    int index;
    unsigned char tabAscii[N_ASCII];
    for(int i=0; i<N_ASCII; i++){   // int and not unsigned char because in the latter case i would never exceeds N_ASCII-1, so it would be an infinite loop
        tabAscii[i]=i;
    }
    for(int i=0; i<buffer->size; i++){
        index = seekChar(tabAscii, N_ASCII, buffer->text[i]);
        buffer->text[i]=index;
        shiftCharStart(tabAscii, N_ASCII, index);
    }
}

/**
 * \fn void moveToFrontDecode(FileBuffer *buffer)
 * \brief Applies the inverse of Move To Front to the given buffer
 * \param buffer Buffer on which is applied the inverse of MTF
*/

void moveToFrontDecode(FileBuffer *buffer)
{
    unsigned char c=0;
    unsigned char tabAscii[N_ASCII];
    for(int i=0; i<N_ASCII; i++){
        tabAscii[i]=i;
    }

    for(int i=0; i<buffer->size; i++){
        c=tabAscii[buffer->text[i]];
        shiftCharStart(tabAscii, N_ASCII, buffer->text[i]);
        buffer->text[i]=c;
    }
}
