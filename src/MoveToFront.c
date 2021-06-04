/**
 * \file MoveToFront.c
 * \brief Applique le Move To Front sur le file transmis a la fonction, ou bien applique son inverse pour revenir au text initial.
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int seekChar(unsigned char tab[], int size, unsigned char c)
 * \brief Cherche l'index de la 1re occurrence du caractere recherche
 * \param tab Tableau dans lequel on cherche le caractere
 * \param size size du tableau tab
 * \param c Caractere qu'on cherche
 * \return index du caractere recherche dans le tableau donne en parametre
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
    fprintf(stderr, "ERREUR : Impossible de trouver le caractere dans chaercherCar dans MTF");
    exit(EXIT_FAILURE);
}


/**
 * \fn void shiftCharStart(unsigned char tab[], int size, int index)
 * \brief Deplace le caractere de l'index donne en parametre au beginning de tab, en decalant tous les caracteres avant
 * \param tab Tableau dans lequel on decale le caractere
 * \param size size du tableau tab
 * \param index index du caractere a decaler
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
 * \brief Applique le Move to front au buffer donne en parametre
 * \param buffer Buffer sur lequel on applique Move to Front
*/

void moveToFrontEncode(FileBuffer *buffer)
{
    int index;
    unsigned char tabAscii[N_ASCII];
    for(int i=0; i<N_ASCII; i++){   // int et non unsigned char car sinon i ne depasse jamais N_ASCII-1
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
 * \brief Applique l'inverse de Move to front au buffer donne en parametre
 * \param buffer Buffer sur lequel on applique l'operation inverse de Move to Front
*/

void moveToFrontDecode(FileBuffer *buffer)
{
    unsigned char c=0;
    unsigned char tabAscii[N_ASCII];
    for(int i=0; i<N_ASCII; i++){   // int et non unsigned char car sinon i ne depasse jamais N_ASCII-1
        tabAscii[i]=i;
    }

    for(int i=0; i<buffer->size; i++){
        c=tabAscii[buffer->text[i]];
        shiftCharStart(tabAscii, N_ASCII, buffer->text[i]);
        buffer->text[i]=c;
    }
}
