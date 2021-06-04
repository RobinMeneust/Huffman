/**
 * \file Decompression.c
 * \brief Decompresse le fichier fichier "compresse.bin" en utilisant la table de codage table.txt precedemment creee.
 * \author Robin Meneust
 * \date 4 juin 2021
 */

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int initialiserTabElementsPossibles(int* tabElements, FILE* fichierTable)
 * \brief Initialise le tableau tabElements pour que seules les cases associees aux caracteres presents dans la fichierTable soient a 1 (et les autres a 0)
 * \param tabElements Tableau contenant les caracteres associes a 1 (s'ils correspondent au code) ou a 0 (dans l'autre cas)
 * \param fichierTable Fichier contenant la table Huffman (caracteres associes a leur code binaire)
 * \return Nombre de cases non nulles
 */

int initialiserTabElementsPossibles(int* tabElements, FILE* fichierTable)
{
    int nbElements=0;
    unsigned char c;
    rewind(fichierTable);
    retourLigneFichier(fichierTable);
    retourLigneFichier(fichierTable);
    retourLigneFichier(fichierTable);
    c=fgetc(fichierTable);
    for(int i=0; i<N_ASCII; i++){
        tabElements[i]=0;
    }
    while(!feof(fichierTable)){
        tabElements[c]=1;
        nbElements++;
        retourLigneFichier(fichierTable);
        c=fgetc(fichierTable);
    }
    return nbElements;
}

/**
 * \fn unsigned char rechPremierIndiceNonNul(int* tabElements)
 * \brief Recherche le premier indice correspondant a une case non nulle dans le tableau donne en parametre
 * \param tabElements Tableau d'entier de N_ASCII cases
 * \return Premier indice du tableau tabElements (caractere) donc la valeur associee est non nulle
 */


unsigned char rechPremierIndiceNonNul(int* tabElements)
{
    for(unsigned char i=0; i<N_ASCII; i++){
        if(tabElements[i]==1){
            return i;
        }
    }
    printf("ERREUR : tableau n'ayant auncun element non nul");
    exit(EXIT_FAILURE);
}

/**
 * \fn actualiserTabElementsPossibles(int* tabElements, FILE* table, uint8_t bit, int position, int* nbElements)
 * \brief Actualise le tableau tabElements en fonction du bit et de la position etudies (en comparant les valeurs du fichier table a ce bit)
 * \param tabElements Tableau contenant les caracteres associes a 1 (s'ils correspondent au code) ou a 0 (dans l'autre cas)
 * \param table Fichier contenant la table Huffman (caracteres associes a leur code binaire)
 * \param bit Bit etudie, a comparer a la valeur binaire a la position etudiee dans le fichier table
 * \param position Position du code etudiee dans le fichier table, nombre positif, 0 correspond a la 1re valeur du code d'une ligne et plus ce nombre est grand plus on se deplace a droite
 * \param nbElements Nombre de cases de tabElements non nulles
 */

void actualiserTabElementsPossibles(int* tabElements, FILE* table, uint8_t bit, int position, int* nbElements)
{
    rewind(table);
    retourLigneFichier(table);
    retourLigneFichier(table);
    retourLigneFichier(table);
    unsigned char c = fgetc(table);
    unsigned char c_table;
    int i=0;

    while(!feof(table)){
        if(tabElements[c]!=0){
            c_table=fgetc(table);
            i=0;
            while(i<position && c_table!='\n' && !feof(table)){
                c_table=fgetc(table);
                i++;
            }
            if(c_table-'0'!=bit){
                tabElements[c]=0;
                (*nbElements)--;
            }
        }
        retourLigneFichier(table);
        c = fgetc(table);
    }
}



/**
 * \fn void decompresser(FILE* fichierEntree, FILE* bufferSortie, FILE* fichierTable)
 * \brief Decompresse le fichierEntree a partir du fichier table.txt
 * \param fichierEntree Fichier texte initial a decompresser
 * \param bufferSortie Buffer decompresse genere par cette fonction
 * \param fichierTable Fichier contenant la table Huffman
 */


void decompresser(FILE* fichierEntree, FileBuffer* bufferSortie, FILE* fichierTable)
{
    int tailleFichierEntree = lireLigneNombre(fichierTable, 2);
    int nbAjouts=0; // nombre de caracteres decompresses
    uint8_t buffer=0;
    uint8_t bit;
    int avancement=0;
    int tailleBuffer;
    int position;
    int tabElementsPossibles[N_ASCII];  // contient 0 ou 1 : 1 = la sequence correspond et 0 sinon
    int nbElementsPossibles = N_ASCII;
    int pos=0;
    bufferSortie->texte = (unsigned char*) malloc(sizeof(unsigned char)*tailleFichierEntree);
    bufferSortie->taille=tailleFichierEntree;
    rewind(fichierEntree);
    buffer=fgetc(fichierEntree);
    position=0;
    tailleBuffer=8;
    nbElementsPossibles = initialiserTabElementsPossibles(tabElementsPossibles, fichierTable);
    while(nbAjouts<tailleFichierEntree && nbElementsPossibles!=0){
        while(tailleBuffer!=0 && nbElementsPossibles!=0 && nbAjouts<tailleFichierEntree){
            bit = buffer & (1<<(tailleBuffer-1));  // on applique un masque pour recuperer le bit a la position tailleBuffer-1 (entre 2^0 et 2^7)
            if(tailleBuffer>1){
                bit >>= tailleBuffer-1;  // on decale vers la droite afin d'avoir la valeur etudiee completement a droite (et donc d'avoir bit valant 0 ou 1)
            }
            actualiserTabElementsPossibles(tabElementsPossibles, fichierTable, bit, position, &nbElementsPossibles);
            position++;
            if(nbElementsPossibles==1){ // il ne reste plus qu'une possibilite, c'est donc le bon caractere
                unsigned char c=rechPremierIndiceNonNul(tabElementsPossibles);
                bufferSortie->texte[pos]=c;
                pos++;
                position=0;
                nbElementsPossibles = initialiserTabElementsPossibles(tabElementsPossibles, fichierTable);
                nbAjouts++;
            }
            tailleBuffer--;
        }

        if(avancement+5<((nbAjouts+1)*100)/tailleFichierEntree)
        {
            avancement=(int)((((nbAjouts+1)*100)/tailleFichierEntree)/5)*5; //Affiche l'etat d'anvancement de la tache
            printf("%d%%\n", avancement);
        }

        buffer=fgetc(fichierEntree);
        tailleBuffer=8;
    }

    if(nbElementsPossibles==0){
        exit(EXIT_FAILURE);
    }
}


/**
 * \fn void decompresserMain(char* nomFichierEntree)
 * \brief Fonction principale pour la decompression : appelle les fonctions necessaires a la decompression du fichier "fichier decompresse.txt" dans le meme repertoire que le programme
 * \param nomFichierEntree Nom du fichier a decompresser
 */


void decompresserMain(char* nomFichierEntree)
{
    FILE* fichierEntree;
    FILE* fichierSortie;
    FILE* fichierTable;
    int indiceBW;

    fichierEntree = fopen(nomFichierEntree, "rb");
    TESTFOPEN(fichierEntree);
    fichierTable = fopen("table.txt", "rb");
    TESTFOPEN(fichierTable);

    indiceBW=lireLigneNombre(fichierTable, 0);

    printf("\nDecompression du fichier selon la table...\n");
    FileBuffer bufferTexte;
    decompresser(fichierEntree, &bufferTexte, fichierTable);
    FCLOSE(fichierEntree);
    
    int option=lireLigneNombre(fichierTable, 1);
    printf("opt : %d\n", option);
    FCLOSE(fichierTable);
    
    int tailleNomFichierEntree = chercherTailleChaine(nomFichierEntree);
    if(nomFichierEntree[tailleNomFichierEntree-4]=='.' && nomFichierEntree[tailleNomFichierEntree-3]=='b' && nomFichierEntree[tailleNomFichierEntree-2]=='i' && nomFichierEntree[tailleNomFichierEntree-1]=='n'){
        nomFichierEntree[tailleNomFichierEntree-4]='\0';  //On enleve le .bin
    }

    //Cas ou le fichier existe deja
    #if __linux__
    if(access(nomFichierEntree, F_OK)!=0){
        printf("ERREUR : Le fichier %s existe deja", nomFichierEntree);
        printf("Saisissez un nom pour le fichier decompresse : \n");
        saisieNomFichier(nomFichierEntree);
    }
    #endif
    #if __WIN32__
    if(_access(nomFichierEntree, 0)!=-1){
        printf("ERREUR : Le fichier %s existe deja", nomFichierEntree);
        printf("Saisissez un nom pour le fichier decompresse : \n");
        saisieNomFichier(nomFichierEntree);
    }
    #endif



    


    fichierSortie = fopen(nomFichierEntree, "wb+"); 
    TESTFOPEN(fichierSortie);
    if(option==1)
    {
        printf("Application de l'inverse de Move To Front...\n");
        moveToFrontDecode(&bufferTexte);

        printf("\nApplication de l'inverse de Burrows Wheeler...\n");
        burrowsWheelerDecode(indiceBW, bufferTexte, fichierSortie);
    }
    else
    {
        chargerDansFichier(bufferTexte,fichierSortie);
    }
    FCLOSE(fichierSortie);
    free(bufferTexte.texte);
    printf("\nLa decompression est terminee\n");
}
