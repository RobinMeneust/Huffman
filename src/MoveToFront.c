/**
 * \file MoveToFront.c
 * \brief Applique le Move To Front sur le fichier transmis a la fonction, ou bien applique son inverse pour revenir au texte initial.
 * \author Robin Meneust
 * \date 4 juin 2021
 */

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int chercherCar(unsigned char tab[], int taille, unsigned char c)
 * \brief Cherche l'indice de la 1re occurrence du caractere recherche
 * \param tab Tableau dans lequel on cherche le caractere
 * \param taille Taille du tableau tab
 * \param c Caractere qu'on cherche
 * \return indice du caractere recherche dans le tableau donne en parametre
 */


int chercherCar(unsigned char tab[], int taille, unsigned char c)
{
    int i=0;

    while(i<taille){
        if(tab[i]==c){
            return i;
        }
        i++;
    }
    fprintf(stderr, "ERREUR : Impossible de trouver le caractere dans chaercherCar dans MTF");
    exit(EXIT_FAILURE);
}


/**
 * \fn void decalerDebCar(unsigned char tab[], int taille, int indice)
 * \brief Deplace le caractere de l'indice donne en parametre au debut de tab, en decalant tous les caracteres avant
 * \param tab Tableau dans lequel on decale le caractere
 * \param taille Taille du tableau tab
 * \param indice Indice du caractere a decaler
 */


void decalerDebCar(unsigned char tab[],int taille, int indice)
{
    int i=indice;
    unsigned char temp=tab[indice];
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
    int indice;
    unsigned char tabAscii[N_ASCII];
    for(int i=0; i<N_ASCII; i++){   // int et non unsigned char car sinon i ne depasse jamais N_ASCII-1
        tabAscii[i]=i;
    }
    for(int i=0; i<buffer->taille; i++){
        indice = chercherCar(tabAscii, N_ASCII, buffer->texte[i]);
        buffer->texte[i]=indice;
        decalerDebCar(tabAscii, N_ASCII, indice);
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

    for(int i=0; i<buffer->taille; i++){
        c=tabAscii[buffer->texte[i]];
        decalerDebCar(tabAscii, N_ASCII, buffer->texte[i]);
        buffer->texte[i]=c;
    }
}
