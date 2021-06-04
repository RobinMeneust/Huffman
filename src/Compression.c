/**
 * \file Compression.c
 * \brief Compresse le file donne par l'utilisateur en utilisant la table de codage table.txt precedemment creee.
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn void compress(FileBuffer bufferBW, FILE* fileOut)
 * \brief Compresse le fileIn a partir de la table generee
 * \param bufferBW text initial a compress
 * \param fileOut file compresse genere par cette fonction
*/

void compress(FileBuffer bufferBW, FILE* fileOut)
{
    uint8_t buffer=0;   // contient les bits en attendant qu'il soit plein, c'est a dire avant d'avoir 8bits a inserer dans le file
    int remplissage=0;
    int progress=0;
    unsigned char c, c_table;
    FILE* table = fopen("table.txt", "rb");
    TESTFOPEN(table);
    rewind(fileOut);
    int pos=0;
    while(pos<bufferBW.size){
        c = bufferBW.text[pos];
        rewind(table);
        wordWrapFile(table);
        wordWrapFile(table);
        wordWrapFile(table);
        c_table = fgetc(table);
        while(c_table!=c && !feof(table)){
            wordWrapFile(table);
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
            if(remplissage==8){ //Le buffer est plein on peut inserer dans le file
                putc(buffer, fileOut);
                buffer=0;
                remplissage=0;
            }
            c_table = fgetc(table);
        }
        pos++;

        if(progress+5<((pos+1)*100)/bufferBW.size)
        {
            progress=(int)((((pos+1)*100)/bufferBW.size)/5)*5; //Affiche l'etat d'anvancement de la tache
            printf("%d%%\n", progress);
        }

    }
    if(remplissage!=0){
        while(remplissage<8 && buffer!=0){
            buffer <<= 1;
            remplissage++;
        }
        putc(buffer, fileOut);
        buffer=0;
        remplissage=0;
    }
    FCLOSE(table);
}


/**
 * \fn void compressMain(char* fileNameIn)
 * \brief Fonction principale pour la compression : appelle les fonctions necessaires a la compression du file dont le nom est en parametre
 * \param fileNameIn Nom du file a compress
*/

void compressMain(char* fileNameIn)
{
    FILE* fileIn;
    FILE* fileOut;
    long fileSizeEntree;
    int indexBW=-1;

    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    fileSizeEntree = seekSizeOfFile(fileIn);
    unsigned char option=0;
    if(fileSizeEntree<1000)
    {
        option=1;
    }

    FileBuffer buffertext=fileToBuffer(fileIn);
    FCLOSE(fileIn);

    if(option==1)
    {
        //BURROWS WHEELER
        printf("\nApplication de Burrows Wheeler...\n");
        indexBW = burrowsWheeler(&buffertext);
        
        // MTF
        printf("\nApplication du Move To Front...\n");
        moveToFrontEncode(&buffertext);
    } 

    //CREATION TABLE
    createHuffmanTable(indexBW, buffertext, option+'0');

    //COMPRESSION
    fileOut = fopen(strncat(fileNameIn, ".bin", 4), "wb+");
    TESTFOPEN(fileOut);
    printf("\nCompression du file...\n");
    compress(buffertext, fileOut);
    printf("\nFin de la compression\n");
    FCLOSE(fileOut);
    free(buffertext.text);
}
