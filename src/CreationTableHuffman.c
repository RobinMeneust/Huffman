/**
 * \file CreationTableHuffman.c
 * \brief Permet la creation de la table de codage Huffman, qui sera enregistree dans un fichier a partir duquel sera faite la compression et la decompression.
 * \author Robin Meneust
 * \date 4 juin 2021
 */


#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn PtrListeCode creerNoeud(unsigned char c)
 * \brief Creer un nouveau noeud pour une liste de type ListeCode
 * \param c Caractere du noeud a creer
 * \return Noeud cree
 */

PtrListeCode creerNoeud(unsigned char c)
{
    ListeCode *noeud = NULL;
    noeud= (ListeCode*) malloc(sizeof(ListeCode));
    TESTALLOC(noeud);
    noeud->valeur = c;
    noeud->suivant = NULL;
    return noeud;
}


/**
 * \fn void ajoutDebListe(PtrListeCode *liste, PtrListeCode noeud)
 * \brief Ajoute un noeud en tete d'une liste de type ListeCode donnee en parametre
 * \param liste Pointeur vers la tete d'une Liste ListeCode dans laquelle la fonction insere le noeud
 * \param noeud Noeud a inserer
 */

void ajoutDebListe(PtrListeCode *liste, PtrListeCode noeud)
{
    if(*liste!=NULL){
        noeud->suivant = *liste;
        *liste = noeud;
    }
    else{
        *liste=noeud;
    }
}


/**
 * \fn void initialiserCode(CaseTableHuffman* tableHuffman,int tailleTabHuffman,CaseTabOccurrences* tabOccurrences)
 * \brief Initialise toutes les listes du champ code d'un tableau de type CaseTableHuffman
 * \param tableHuffman Tableau dont il faut initialiser les listes
 * \param tailleTabHuffman Taille du tableau dynamique tableHuffman
 * \param tabOccurrences Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 */


void initialiserCode(CaseTableHuffman* tableHuffman,int tailleTabHuffman,CaseTabOccurrences* tabOccurrences)
{
    for(int i=0; i<tailleTabHuffman; i++){
        tableHuffman[i].code=NULL;
        tableHuffman[i].c=tabOccurrences[i].c[0];
    }
}


/**
 * \fn CaseTabOccurrences* remplirTabOccurrences(FILE* fichier,int* tailleTabOccurrences)
 * \brief Fonction allouant et initialisant le tableau tabOccurrences associant chaque caractere a son nombre d'occurrences
 * \param buffer Buffer a partir duquel cette fonction remplit le tableau donne en parametre
 * \param tailleTabOccurrences Taille du tableau dynamique tabOccurrences(ici t_retour), est incremente pendant que le tableau est construit
 * \return Tableau de structures CaseTabOccurrences contenant chaque caractere associe a son nombre d'occurrence
 */


CaseTabOccurrences* remplirTabOccurrences(FileBuffer buffer,int* tailleTabOccurrences)
{
    int car[N_ASCII]={0};
    unsigned char c;
    *tailleTabOccurrences=0;
    CaseTabOccurrences* t_retour = NULL;
    int pos=0;
    while(pos<buffer.taille)
    {
        c=buffer.texte[pos];
        car[c]++;
        pos++;
    }

    int idcar=0;
    for(int i=0;i<N_ASCII;i++)
    {
        if(car[i]!=0)
        {
            (*tailleTabOccurrences)++;
            t_retour=(CaseTabOccurrences*)realloc(t_retour,sizeof(CaseTabOccurrences)*(*tailleTabOccurrences));
            TESTALLOC(t_retour);
            t_retour[idcar].c = (unsigned char*) malloc(sizeof(unsigned char));
            TESTALLOC(t_retour[idcar].c);
            t_retour[idcar].taille=1;
            t_retour[idcar].c[0]=i;
            t_retour[idcar].occurrences=car[i];
            idcar++;
        }
    }
    return t_retour;
}

/**
 * \fn void rech2Min(int* i_min1, int* i_min2, CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences)
 * \brief Cherche les 2 elements du tableau tabOccurrences donnee en entree avec le champ occurrences minimal (les moins frequents)
 * \param i_min1 pointeur vers l'indice du 1er minimum du champ occurrence du tableau
 * \param i_min2 pointeur vers l'indice du 2e minimum du champ occurrence du tableau
 * \param tabOccurrences Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param tailleTabOccurrences Taille du tableau dynamique tabOccurrences
 */


void rech2Min(int* i_min1, int* i_min2, CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences)
{
    for(int i=2; i<tailleTabOccurrences; i++){
        if(tabOccurrences[i].occurrences < tabOccurrences[*i_min2].occurrences){
            *i_min2 = i;
            if (tabOccurrences[*i_min2].occurrences < tabOccurrences[*i_min1].occurrences){
                int tmp=*i_min2;
                *i_min2 = *i_min1;
                *i_min1 = tmp;
            }
        }
    }
}

/**
 * \fn void fusionner(int i_min1, int i_min2, CaseTabOccurrences* tabOccurrences,int* tailleTabOccurrences)
 * \brief Fusionne 2 cases d'un tableau de structures CaseTabOccurrences, decale la suite du tableau sur la case copiee et realloue pour diminuer la taille du tableau
 * \param i_min1 l'indice du 1er minimum du champ occurrence du tableau
 * \param i_min2 l'indice du 2e minimum du champ occurrence du tableau
 * \param tabOccurrences Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param tailleTabOccurrences Taille du tableau dynamique tabOccurrences
 */

void fusionner(int i_min1, int i_min2, CaseTabOccurrences* tabOccurrences,int* tailleTabOccurrences)
{
    tabOccurrences[i_min1].c=(unsigned char*)realloc(tabOccurrences[i_min1].c,sizeof(unsigned char)*(tabOccurrences[i_min1].taille+tabOccurrences[i_min2].taille));
    TESTALLOC(tabOccurrences[i_min1].c);
    for(int i=tabOccurrences[i_min1].taille;i<tabOccurrences[i_min1].taille+tabOccurrences[i_min2].taille;i++)
    {
        tabOccurrences[i_min1].c[i]=tabOccurrences[i_min2].c[i-tabOccurrences[i_min1].taille];
    }
    tabOccurrences[i_min1].taille+=tabOccurrences[i_min2].taille;

    tabOccurrences[i_min1].occurrences+= tabOccurrences[i_min2].occurrences;


    tabOccurrences[i_min2]=tabOccurrences[(*tailleTabOccurrences)-1];
    (*tailleTabOccurrences)--;
    tabOccurrences=(CaseTabOccurrences*)realloc(tabOccurrences,*tailleTabOccurrences*sizeof(CaseTabOccurrences));
    TESTALLOC(tabOccurrences);
}


/**
 * \fn void remplirCodeTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman, CaseTabOccurrences* tabOccurrences, int i_min1, int i_min2)
 * \brief Remplit les cases de la tableHuffman correspondantes aux caracteres contenus dans le champ c de tabOcurrences pour les cases i_min1 et i_min2 avec 0 ou 1
 * \param tableHuffman Tableau de structures CaseTableHuffman contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le texte
 * \param tailleTabHuffman Taille du tableau dynamique tableHuffman
 * \param tabOccurrences Tableau de structures contenant chaque caractere associe a son nombre d'occurrence
 * \param i_min1 l'indice du 1er minimum du champ occurrence du tableau
 * \param i_min2 l'indice du 2e minimum du champ occurrence du tableau
 */


void remplirCodeTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman, CaseTabOccurrences* tabOccurrences, int i_min1, int i_min2)
{
    int j;
    for(int i=0;i<tabOccurrences[i_min1].taille;i++){
        j=0;
        while(tabOccurrences[i_min1].c[i]!=tableHuffman[j].c && j<tailleTabHuffman){
            j++;
        }
        if(tabOccurrences[i_min1].c[i]==tableHuffman[j].c)
            ajoutDebListe(&(tableHuffman[j].code), creerNoeud('0'));
    }

    for(int i=0;i<tabOccurrences[i_min2].taille;i++){
        j=0;
        while(tabOccurrences[i_min2].c[i]!=tableHuffman[j].c && j<tailleTabHuffman){
            j++;
        }
        if(tabOccurrences[i_min2].c[i]==tableHuffman[j].c)
            ajoutDebListe(&(tableHuffman[j].code), creerNoeud('1'));
    }
}



/**
 * \fn void libererTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman)
 * \brief Libere la memoire occupee par la tableHuffman y compris ses listes (du champ code)
 * \param tableHuffman Tableau de structures CaseTableHuffman contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le texte
 * \param tailleTabHuffman Taille du tableau dynamique tableHuffman
 */


void libererTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman)
{
    PtrListeCode p = NULL;
    for(int i=0; i<tailleTabHuffman; i++){
        while(tableHuffman[i].code!=NULL){
            p = tableHuffman[i].code;
            tableHuffman[i].code = tableHuffman[i].code->suivant;
            free(p);
        }
    }
    free(tableHuffman);
}

/**
 * \fn void libererTabOccurrences(CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences)
 * \brief Libere la memoire occupee par la tabOccurrences y compris ses tableaux dynamiques (du champ c)
 * \param tabOccurrences Tableau de structures CaseTabOccurrences contenant tous les caracteres associes a un nombre d'occurrences dans le texte
 * \param tailleTabOccurrences Taille du tableau dynamique tabOccurrences
 */


void libererTabOccurrences(CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences)
{
    for(int i=0; i<tailleTabOccurrences; i++)
        free(tabOccurrences[i].c);
    free(tabOccurrences);
}



/**
 * \fn void enregistrerTable(int indiceBW CaseTableHuffman* tableHuffman,int tailleTabHuffman, int tailleFichier, unsigned char option)
 * \brief Enregistre la table de codage dans un fichier texte
 * \param indiceBW Indice permettant de decoder le texte BW
 * \param tableHuffman Tableau de structures CaseTableHuffman contenant tous les caracteres associes a une liste de 0 ou de 1 selon leur occurrence dans le texte
 * \param tailleTabHuffman Taille du tableau dynamique tableHuffman
 * \param tailleFichier Nombre de caracteres du fichier a compresser
 * \param option Contient 0 si on ne doit pas appliquer BW et MTF ou 1 si on doit le faire
*/


void enregistrerTable(int indiceBW, CaseTableHuffman* tableHuffman,int tailleTabHuffman, int tailleFichier, unsigned char option)
{
    FILE* fichierTable = fopen("table.txt", "wb+");
    TESTFOPEN(fichierTable);
    PtrListeCode l=NULL;
    fprintf(fichierTable, "%d\n%c\n%d\n", indiceBW, option, tailleFichier);
    printf("DEB\n\n%c\n%d\n\n", option, tailleFichier);
    for(int i=0; i<tailleTabHuffman; i++){
        l = tableHuffman[i].code;
        fputc(tableHuffman[i].c, fichierTable);
        printf("TEST : i:%d     c   %c:%d\n", i, tableHuffman[i].c, tableHuffman[i].c);
        while(l!=NULL){
            fputc(l->valeur, fichierTable);
            l = l->suivant;
        }
        fputc('\n', fichierTable);
    }
    FCLOSE(fichierTable);
}



/**
 * \fn void creerTableHuffman(int indiceBW, FileBuffer bufferEntree, unsigned char option)
 * \brief Fonction creant, remplissant et sauvegardant la tableHuffman associant chaque caracteres present dans le fichier a un nombre en binaire
 * \param indiceBW Indice permettant de decoder le texte BW
 * \param bufferEntree Buffer dont on cree la table
 * \param option Contient 0 si on ne doit pas appliquer BW et MTF ou 1 si on doit le faire
*/

void creerTableHuffman(int indiceBW, FileBuffer bufferEntree, unsigned char option)
{
    int tailleTabOccurrences;
    CaseTabOccurrences* tabOccurrences = remplirTabOccurrences(bufferEntree,&tailleTabOccurrences);

    int tailleTabHuffman=tailleTabOccurrences;
    CaseTableHuffman* tableHuffman = NULL;
    tableHuffman = (CaseTableHuffman*) malloc(tailleTabHuffman*sizeof(CaseTableHuffman));
    TESTALLOC(tableHuffman);
    initialiserCode(tableHuffman,tailleTabHuffman,tabOccurrences);

    while(tailleTabOccurrences>1){  // Jusqu'a qu'il ne reste plus qu'un element
        int i_min1 = 0;
        int i_min2 = 1;
        if (tabOccurrences[i_min2].occurrences < tabOccurrences[i_min1].occurrences){
            int tmp=i_min2;
            i_min2 = i_min1;
            i_min1 = tmp;
        }
        rech2Min(&i_min1, &i_min2, tabOccurrences,tailleTabOccurrences);
        remplirCodeTableHuffman(tableHuffman, tailleTabHuffman, tabOccurrences, i_min1, i_min2);
        fusionner(i_min1, i_min2, tabOccurrences,&tailleTabOccurrences);
    }

    enregistrerTable(indiceBW, tableHuffman, tailleTabHuffman, bufferEntree.taille, option);

    libererTableHuffman(tableHuffman,tailleTabHuffman);
    libererTabOccurrences(tabOccurrences,tailleTabOccurrences);
}
