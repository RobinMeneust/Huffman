/**
 * \file Structures_Define.h
 * \brief Contient les inclusion de librairie, les constantes, les macros et les structures du projet
 * \author Robin Meneust
 * \date 4 juin 2021
 */


#ifndef StructuresDefine
#define StructuresDefine

//Inclusion des entete de libraire
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Entete de libraire pour le type uint8_t pour Compression.c et Decompression.c
#include <inttypes.h>

//Entete de libraire pour la fonction access utilisee dans DecompressionMain
#if __linux__
#include <unistd.h>
#endif
#if __WIN32__
#include <io.h>
#endif



//Definition de constantes

/**
 * \def N_ASCII Constante correpondant au nombre de valeurs possibles prises par un octet
 */

#define N_ASCII 256

/**
 * \def FCLOSE(X) Macro verifiant la fermeture d'un fichier, si elle n'a pas fonctionne on arrete le programme
 */

#define FCLOSE(X){\
            if(fclose(X)==EOF){\
                fprintf(stderr, "ERREUR : Fermeture du fichier impossible"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \def TESTFOPEN(X) Macro verifiant l'ouverture d'un fichier, si elle n'a pas fonctionne on arrete le programme
 */

#define TESTFOPEN(X){\
            if(X==NULL){\
                fprintf(stderr, "ERREUR : Ouverture du fichier impossible"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \def TESTALLOC(X) Macro verifiant si un pointeur a bien ete associe a une adresse suite a une allocation dynamique, si ce n'est pas le cas on arrete le programme
 */

#define TESTALLOC(X){\
            if(X==NULL){\
                fprintf(stderr, "ERREUR : Allocation impossible"); \
                exit(EXIT_FAILURE); \
            }\
}

/**
 * \struct FileBuffer Structures_Define.h
 * \brief Buffer contenant un fichier ou du texte
 */

typedef struct FileBuffer{
    unsigned char* texte; /*!< texte contenu dans le buffer */
    int taille; /*!< taille du buffer (nombre de caracteres) */
}FileBuffer;

/**
 * \struct ListeCode Structures_Define.h
 * \brief Noeud d'une liste contenant une suite de caracteres 0 ou 1
 */

typedef struct ListeCode{
    unsigned char valeur; /*!< caractere contenu dans le noeud (0 ou 1) */
    struct ListeCode *suivant; /*!< pointeur vers le noeud suivant */
}ListeCode;

typedef ListeCode* PtrListeCode;

/**
 * \struct CaseTabOccurrences Structures_Define.h
 * \brief Case d'un tableau associant a des caracteres leur nombre d'occurrences et leur nombre dans la case (taille de la chaine c)
 */

typedef struct CaseTabOccurrences{
    int occurrences; /*!< nombre d'occurrences des caracteres contenus dans c*/
    unsigned char* c; /*!< chaine de caracteres */
    int taille; /*!< taille de la chaine c*/
}CaseTabOccurrences;

/**
 * \struct CaseTableHuffman Structures_Define.h
 * \brief Case d'un tableau associant a un caractere un code binaire.
 */

typedef struct CaseTableHuffman{
    unsigned char c; /*!< caractere auquel on associe le code*/
    PtrListeCode code; /*!< liste chainee contenant le code binaire associe a c*/
}CaseTableHuffman;

#endif
