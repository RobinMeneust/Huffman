/**
 * \file HuffmanTableCreation.c
 * \brief Permet la creation de la table de codage Huffman, qui sera enregistree dans un file a partir duquel sera faite la compression et la decompression.
 * \author Robin Meneust
 * \date 2021
*/


#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn PtrlistCode createNode(unsigned char c)
 * \brief Creer un nouveau node pour une liste de type listCode
 * \param c Caractere du node a creer
 * \return node cree
*/

PtrlistCode createNode(unsigned char c)
{
    listCode *node = NULL;
    node= (listCode*) malloc(sizeof(listCode));
    TESTALLOC(node);
    node->valeur = c;
    node->next = NULL;
    return node;
}


/**
 * \fn void addStartList(PtrlistCode *liste, PtrlistCode node)
 * \brief Ajoute un node en tete d'une liste de type listCode donnee en parametre
 * \param liste Pointeur vers la tete d'une liste listCode dans laquelle la fonction insere le node
 * \param node node a inserer
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
 * \brief Initialise toutes les listes du champ code d'un tableau de type HuffmanTableCell
 * \param huffmanTable Tableau dont il faut initialiser les listes
 * \param sizeHuffmanTable size du tableau dynamique huffmanTable
 * \param occurrencesArray Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
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
 * \brief Fonction allouant et initialisant le tableau occurrencesArray associant chaque caractere a son nombre d'occurrences
 * \param buffer Buffer a partir duquel cette fonction remplit le tableau donne en parametre
 * \param occurrencesArraySize size du tableau dynamique occurrencesArray(ici t_return), est incremente pendant que le tableau est construit
 * \return Tableau de structures OccurrencesArrayCell contenant chaque caractere associe a son nombre d'occurrence
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
 * \brief Cherche les 2 elements du tableau occurrencesArray donnee en entree avec le champ occurrences minimal (les moins frequents)
 * \param i_min1 pointeur vers l'index du 1er minimum du champ occurrence du tableau
 * \param i_min2 pointeur vers l'index du 2e minimum du champ occurrence du tableau
 * \param occurrencesArray Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param occurrencesArraySize size du tableau dynamique occurrencesArray
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
 * \brief Fusionne 2 cases d'un tableau de structures OccurrencesArrayCell, decale la suite du tableau sur la case copiee et realloue pour diminuer la size du tableau
 * \param i_min1 l'index du 1er minimum du champ occurrence du tableau
 * \param i_min2 l'index du 2e minimum du champ occurrence du tableau
 * \param occurrencesArray Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param occurrencesArraySize size du tableau dynamique occurrencesArray
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
 * \brief Remplit les cases de la huffmanTable correspondantes aux caracteres contenus dans le champ c de tabOcurrences pour les cases i_min1 et i_min2 avec 0 ou 1
 * \param huffmanTable Tableau de structures HuffmanTableCell contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le text
 * \param sizeHuffmanTable size du tableau dynamique huffmanTable
 * \param occurrencesArray Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param i_min1 l'index du 1er minimum du champ occurrence du tableau
 * \param i_min2 l'index du 2e minimum du champ occurrence du tableau
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
 * \brief Libere la memoire occupee par la huffmanTable y compris ses listes (du champ code)
 * \param huffmanTable Tableau de structures HuffmanTableCell contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le text
 * \param sizeHuffmanTable size du tableau dynamique huffmanTable
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
 * \brief Libere la memoire occupee par la occurrencesArray y compris ses tableaux dynamiques (du champ c)
 * \param occurrencesArray Tableau de structures OccurrencesArrayCell contenant tous les caracteres associes a un nombre d'occurrences dans le text
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
 * \brief Enregistre la table de codage dans un file text
 * \param indexBW index permettant de decoder le text BW
 * \param huffmanTable Tableau de structures HuffmanTableCell contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le text
 * \param sizeHuffmanTable size du tableau dynamique huffmanTable
 * \param fileSize Nombre de caracteres du file a compress
 * \param option Contient 0 si on ne doit pas appliquer BW et MTF ou 1 si on doit le faire
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
        printf("TEST : i:%d     c   %c:%d\n", i, huffmanTable[i].c, huffmanTable[i].c);
        while(l!=NULL){
            fputc(l->valeur, fileTable);
            l = l->next;
        }
        fputc('\n', fileTable);
    }
    FCLOSE(fileTable);
}



/**
 * \fn void createHuffmanTable(int indexBW, FileBuffer bufferIn, unsigned char option)
 * \brief Fonction creant, remplissant et sauvegardant la huffmanTable associant chaque caracteres present dans le file a un nombre en binaire
 * \param indexBW index permettant de decoder le text BW
 * \param bufferIn Buffer dont on cree la table
 * \param option Contient 0 si on ne doit pas appliquer BW et MTF ou 1 si on doit le faire
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

    while(occurrencesArraySize>1){  // Jusqu'a qu'il ne reste plus qu'un element
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
