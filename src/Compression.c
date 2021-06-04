/**
 * \file Compression.c
 * \brief Compresse le fichier donne par l'utilisateur en utilisant la table de codage table.txt precedemment creee.
 * \author Robin Meneust
 * \date 4 juin 2021
 */

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn void compresser(FileBuffer bufferBW, FILE* fichierSortie)
 * \brief Compresse le fichierEntree a partir de la table generee
 * \param bufferBW Texte initial a compresser
 * \param fichierSortie Fichier compresse genere par cette fonction
 */

void compresser(FileBuffer bufferBW, FILE* fichierSortie)
{
    uint8_t buffer=0;   // contient les bits en attendant qu'il soit plein, c'est a dire avant d'avoir 8bits a inserer dans le fichier
    int remplissage=0;
    int avancement=0;
    unsigned char c, c_table;
    FILE* table = fopen("table.txt", "rb");
    TESTFOPEN(table);
    rewind(fichierSortie);
    int pos=0;
    while(pos<bufferBW.taille){
        c = bufferBW.texte[pos];
        rewind(table);
        retourLigneFichier(table);
        retourLigneFichier(table);
        retourLigneFichier(table);
        c_table = fgetc(table);
        while(c_table!=c && !feof(table)){
            retourLigneFichier(table);
            c_table = fgetc(table);
        }
        if(c_table!=c){
                printf("ERREUR : Caractere non trouve dans la table:%c|%d",c,c);
            exit(EXIT_FAILURE);
        }

        c_table = fgetc(table);
        while(c_table!='\n' && !feof(table)){
            switch(c_table){
                case '0': buffer <<= 1; break;  //On decale les bits vers la gauche puis on ajoute le bit (ici 0) tout a droite en utilisant un ou (car 100 << 1 donne 1000)
                case '1': buffer <<= 1; buffer|=0b1; break; //On decale les bits vers la gauche puis on ajoute le bit (ici 1) tout a droite en utilisant un "ou" (car 100 << 1 donne 1000 et on fait un "ou" (avec le masque 00000001) ce qui donne 1001)
            }
            remplissage++;
            if(remplissage==8){ //Le buffer est plein on peut inserer dans le fichier
                putc(buffer, fichierSortie);
                buffer=0;
                remplissage=0;
            }
            c_table = fgetc(table);
        }
        pos++;

        if(avancement+5<((pos+1)*100)/bufferBW.taille)
        {
            avancement=(int)((((pos+1)*100)/bufferBW.taille)/5)*5; //Affiche l'etat d'anvancement de la tache
            printf("%d%%\n", avancement);
        }

    }
    if(remplissage!=0){
        while(remplissage<8 && buffer!=0){
            buffer <<= 1;
            remplissage++;
        }
        putc(buffer, fichierSortie);
        buffer=0;
        remplissage=0;
    }
    FCLOSE(table);
}


/**
 * \fn void compresserMain(char* nomFichierEntree)
 * \brief Fonction principale pour la compression : appelle les fonctions necessaires a la compression du fichier dont le nom est en parametre
 * \param nomFichierEntree Nom du fichier a compresser
 */

void compresserMain(char* nomFichierEntree)
{
    FILE* fichierEntree;
    FILE* fichierSortie;
    long tailleFichierEntree;
    int indiceBW=-1;

    fichierEntree = fopen(nomFichierEntree, "rb");
    TESTFOPEN(fichierEntree);
    tailleFichierEntree = chercherTaille(fichierEntree);
    unsigned char option=0;
    if(tailleFichierEntree<1000)
    {
        option=1;
    }

    FileBuffer bufferTexte=chargerDansBuffer(fichierEntree);
    FCLOSE(fichierEntree);

    if(option==1)
    {
        //BURROWS WHEELER
        printf("\nApplication de Burrows Wheeler...\n");
        indiceBW = burrowsWheeler(&bufferTexte);
        
        // MTF
        printf("\nApplication du Move To Front...\n");
        moveToFrontEncode(&bufferTexte);
    } 

    //CREATION TABLE
    creerTableHuffman(indiceBW, bufferTexte, option+'0');

    //COMPRESSION
    fichierSortie = fopen(strncat(nomFichierEntree, ".bin", 4), "wb+");
    TESTFOPEN(fichierSortie);
    printf("\nCompression du fichier...\n");
    compresser(bufferTexte, fichierSortie);
    printf("\nFin de la compression\n");
    FCLOSE(fichierSortie);
    free(bufferTexte.texte);
}
