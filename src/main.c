/**
 * \file main.c
 * \brief Programme principal du projet. Permet le lancement de la compression ou de la decompression.
 * \author Robin Meneust
 * \date 2021
*/


#include "../include/FonctionsHuffman.h"
#include "../include/Structures_Define.h"

/**
 * \fn int main(int argc, char *argv[])
 * \brief Fonction principale permettant de compress ou decompress un file donne en parametre, s'il n'y en a pas on redemande a l'utilisateur de saisir ce nom
*/

int main(int argc, char *argv[])
{
    char fileNameIn[FILENAME_MAX];
    int choix=0;

    //Choix entre compression, decompression et sortie du programme
    printf("MENU\n\n");
    printf("0 : Arreter le programme\n");
    printf("1 : compress un file\n");
    printf("2 : Decompress un file\n\n");
    printf("Entrez le chiffre correpondant a l'operation a effectuer : ");
    choix = fgetc(stdin)-'0';
    while(choix<0 || choix>2){
        printf("Veuillez entrez un chiffre entre 0 et 2 inclus : ");
        choix = fgetc(stdin)-'0';
    }
    getchar();
    printf("\nCHOIX %d :\n", choix);

    if(choix!=0){
        //Application de la fonction choisie
        switch(choix){
            case 1 :
                // Recuperation du nom de file a ouvrir
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// si la size de la string est correcte pour etre copie dans fileNameIn et s'il y a 2 arguments, car le premier est toujours le nom de l'executable
                    sprintf(fileNameIn, "%s", argv[1]);
                else{
                    getFileName(fileNameIn);
                }

                if(!strcmp(fileNameIn, "table.txt")){ // le file d'entree ne doit pas avoir le meme nom que d'autres files utilises dans le programme
                    fprintf(stderr, "ERREUR : Le nom du file doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                compressMain(fileNameIn);
                break;
            case 2 :
                // Recuperation du nom de file a ouvrir
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// si la size de la string est correcte pour etre copie dans fileNameIn et s'il y a 2 arguments, car le premier est toujours le nom de l'executable
                    sprintf(fileNameIn, "%s", argv[1]);
                else{
                    getFileName(fileNameIn);
                }

                if(!strcmp(fileNameIn, "table.txt")){ // le file d'entree ne doit pas avoir le meme nom que d'autres files utilises dans le programme
                    fprintf(stderr, "ERREUR : Le nom du file doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                decompressMain(fileNameIn);
                break;
            default :
                fprintf(stderr, "ERREUR : Choix incorrect");
                exit(EXIT_FAILURE);
        }
    }
    return 0;
}
