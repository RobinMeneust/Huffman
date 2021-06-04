/**
 * \file FonctionsGlobales.c
 * \brief Contient des fonctions utilisees plusieurs fois par differentes fonctions.
 * \author Robin Meneust
 * \date 4 juin 2021
 */

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"
 


/**
 * \fn void saisieNomFichier(char * nomFichier)
 * \brief Permet la saisie d'un nom de fichier
 * \param nomFichier Nom saisi
*/

void saisieNomFichier(char * nomFichier)
{
    char* positionFinSaisie = NULL;
    printf("\nNom complet du fichier a compresser ou decompresser (avec son extension s'il en a) : ");
    if(fgets(nomFichier, FILENAME_MAX, stdin)==NULL){
        fprintf(stderr, "ERREUR : Probleme lors de la recuperation du nom de fichier");
        exit(EXIT_FAILURE);
    }
    positionFinSaisie = strchr(nomFichier, '\n'); // on cherche la position du carractere \n pour le supprimer du nom de fichier et qu'il marque la fin de ce dernier
    if(positionFinSaisie!=NULL){
        *positionFinSaisie='\0';
    }
}





/**
 * \fn int chercherTailleChaine(char * chaine)
 * \brief Donne la taille de la chaine donnee
 * \param chaine Chaine lue
 * \return Taille de la chaine lue
*/


int chercherTailleChaine(char * chaine)
{
    int i=0;
    while(chaine[i]!='\0')
        i++;
    return i;
}






/**
 * \fn void chargerDansFichier(FileBuffer buffer,FILE* fichier)
 * \brief Ecrit le contenu du buffer dans le fichier
 * \param buffer Buffer lu
 * \param fichier Fichier ou l'on ecrit le buffer
 */

void chargerDansFichier(FileBuffer buffer,FILE* fichier)
{
    rewind(fichier);
    for(int i=0;i<buffer.taille;i++)
    {
        fputc(buffer.texte[i],fichier);
    }
}

/**
 * \fn FileBuffer chargerDansBuffer(FILE* fichier)
 * \brief Ecrit un fichier dans le buffer
 * \param fichier Fichier lu
 * \return Buffer contenant le fichier qu'on a charge dedans
 */

FileBuffer chargerDansBuffer(FILE* fichier)
{
    FileBuffer buffer;
    buffer.taille=chercherTaille(fichier);
    buffer.texte=malloc(buffer.taille*sizeof(unsigned char));
    TESTALLOC(buffer.texte);
    rewind(fichier);
    for(int i=0;i<buffer.taille;i++)
    {
        buffer.texte[i]=fgetc(fichier);
    }
    return buffer;
}

/**
 * \fn long lireLigneNombre(FILE* fichier, long ligne)
 * \brief Donne la taille contenue dans le debut (1re ligne) du fichier (valeur entiere)
 * \param fichier Fichier dont on extrait la taille
 * \param ligne Ligne dont on extrait le nombre
 * \return Nombre extrait du fichier a la ligne donnee
 */

long lireLigneNombre(FILE* fichier, long ligne)
{
    int c;
    long valeur=0;
    long nbLignesLues=0;

    rewind(fichier);

    while(nbLignesLues<ligne){
        retourLigneFichier(fichier);
        nbLignesLues++;
    }

    while(((c=fgetc(fichier))!=EOF) && c!='\n'){
        valeur=(valeur*10)+c-'0';
    }
    return valeur;
}

/**
 * \fn long chercherTailleDansBuffer(FileBuffer buffer)
 * \brief Donne la taille contenue dans le debut (1re ligne) du buffer (valeur entiere)
 * \param buffer Buffer dont on extrait la taille
 * \return Taille extraite du buffer
 */

long chercherTailleDansBuffer(FileBuffer buffer)
{
    return strtol(buffer.texte,NULL, 10); // car strtol ne va pas lire \n et apres (car ce n'est pas un chiffre)
}

/**
 * \fn int retourLigneBuffer(FileBuffer buffer)
 * \brief Passe au debut de la ligne suivante du buffer donne en parametre
 * \param buffer Buffer lu
 */

int retourLigneBuffer(FileBuffer buffer)
{
    int pos=0;
    unsigned char c = buffer.texte[pos];
    pos++;
    while(c!='\n' && pos<buffer.taille){
        c = buffer.texte[pos];
        pos++;
    }
    return pos;
}

/**
 * \fn void retourLigneFichier(FILE* fichier)
 * \brief Passe au debut de la ligne suivante du fichier donne en parametre
 * \param fichier Fichier lu
 */

void retourLigneFichier(FILE* fichier)
{
    int c;
    while(((c = fgetc(fichier))!=EOF) && c!='\n');
}

/**
 * \fn long chercherTaille(FILE* fichier)
 * \brief Fonction donnant le nombre de caracteres du fichier lu
 * \param fichier Fichier lu
 * \return Nombre de caracteres total dans le fichier
 */

int chercherTaille(FILE* fichier)
{
    long taille=0;
    fseek(fichier, 0L, SEEK_END);
    taille = ftell(fichier);
    rewind(fichier);
    return taille;
}
