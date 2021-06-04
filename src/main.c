/**
 * \file main.c
 * \brief Programme principal du projet. Permet le lancement de la compression ou de la decompression.
 * \author Robin Meneust
 * \date 4 juin 2021
 */


#include "../include/FonctionsHuffman.h"
#include "../include/Structures_Define.h"

/**
 * \fn int main(int argc, char *argv[])
 * \brief Fonction principale permettant de compresser ou decompresser un fichier donne en parametre, s'il n'y en a pas on redemande a l'utilisateur de saisir ce nom
 */

int main(int argc, char *argv[])
{
    char nomFichierEntree[FILENAME_MAX];
    int choix=0;

    //Choix entre compression, decompression et sortie du programme
    printf("MENU\n\n");
    printf("0 : Arreter le programme\n");
    printf("1 : Compresser un fichier\n");
    printf("2 : Decompresser un fichier\n\n");
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
                // Recuperation du nom de fichier a ouvrir
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// si la taille de la chaine est correcte pour etre copie dans nomFichierEntree et s'il y a 2 arguments, car le premier est toujours le nom de l'executable
                    sprintf(nomFichierEntree, "%s", argv[1]);
                else{
                    saisieNomFichier(nomFichierEntree);
                }

                if(!strcmp(nomFichierEntree, "table.txt")){ // le fichier d'entree ne doit pas avoir le meme nom que d'autres fichiers utilises dans le programme
                    fprintf(stderr, "ERREUR : Le nom du fichier doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                compresserMain(nomFichierEntree);
                break;
            case 2 :
                // Recuperation du nom de fichier a ouvrir
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// si la taille de la chaine est correcte pour etre copie dans nomFichierEntree et s'il y a 2 arguments, car le premier est toujours le nom de l'executable
                    sprintf(nomFichierEntree, "%s", argv[1]);
                else{
                    saisieNomFichier(nomFichierEntree);
                }

                if(!strcmp(nomFichierEntree, "table.txt")){ // le fichier d'entree ne doit pas avoir le meme nom que d'autres fichiers utilises dans le programme
                    fprintf(stderr, "ERREUR : Le nom du fichier doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                decompresserMain(nomFichierEntree);
                break;
            default :
                fprintf(stderr, "ERREUR : Choix incorrect");
                exit(EXIT_FAILURE);
        }
    }
    return 0;
}
