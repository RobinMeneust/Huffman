/**
 * \file GlobalFunctions.c
 * \brief Contient des fonctions utilisees plusieurs fois par differentes fonctions.
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"
 


/**
 * \fn void getFileName(char * fileName)
 * \brief Permet la saisie d'un nom de file
 * \param fileName Nom saisi
*/

void getFileName(char * fileName)
{
    char* positionFinSaisie = NULL;
    printf("\nNom complet du file a compress ou decompress (avec son extension s'il en a) : ");
    if(fgets(fileName, FILENAME_MAX, stdin)==NULL){
        fprintf(stderr, "ERREUR : Probleme lors de la recuperation du nom de file");
        exit(EXIT_FAILURE);
    }
    positionFinSaisie = strchr(fileName, '\n'); // on cherche la position du carractere \n pour le supprimer du nom de file et qu'il marque la fin de ce dernier
    if(positionFinSaisie!=NULL){
        *positionFinSaisie='\0';
    }
}





/**
 * \fn int seekStringSize(char * string)
 * \brief Donne la size de la string donnee
 * \param string string lue
 * \return size de la string lue
*/


int seekStringSize(char * string)
{
    int i=0;
    while(string[i]!='\0')
        i++;
    return i;
}






/**
 * \fn void bufferToFile(FileBuffer buffer,FILE* file)
 * \brief Ecrit le contenu du buffer dans le file
 * \param buffer Buffer lu
 * \param file file ou l'on ecrit le buffer
*/

void bufferToFile(FileBuffer buffer,FILE* file)
{
    rewind(file);
    for(int i=0;i<buffer.size;i++)
    {
        fputc(buffer.text[i],file);
    }
}

/**
 * \fn FileBuffer fileToBuffer(FILE* file)
 * \brief Ecrit un file dans le buffer
 * \param file file lu
 * \return Buffer contenant le file qu'on a charge dedans
*/

FileBuffer fileToBuffer(FILE* file)
{
    FileBuffer buffer;
    buffer.size=seekSizeOfFile(file);
    buffer.text=malloc(buffer.size*sizeof(unsigned char));
    TESTALLOC(buffer.text);
    rewind(file);
    for(int i=0;i<buffer.size;i++)
    {
        buffer.text[i]=fgetc(file);
    }
    return buffer;
}

/**
 * \fn long readNumberLine(FILE* file, long line)
 * \brief Donne la size contenue dans le beginning (1re line) du file (valeur entiere)
 * \param file file dont on extrait la size
 * \param line line dont on extrait le nombre
 * \return Nombre extrait du file a la line donnee
*/

long readNumberLine(FILE* file, long line)
{
    int c;
    long valeur=0;
    long nblinesLues=0;

    rewind(file);

    while(nblinesLues<line){
        wordWrapFile(file);
        nblinesLues++;
    }

    while(((c=fgetc(file))!=EOF) && c!='\n'){
        valeur=(valeur*10)+c-'0';
    }
    return valeur;
}

/**
 * \fn long seekSizeInBuffer(FileBuffer buffer)
 * \brief Donne la size contenue dans le beginning (1re line) du buffer (valeur entiere)
 * \param buffer Buffer dont on extrait la size
 * \return size extraite du buffer
*/

long seekSizeInBuffer(FileBuffer buffer)
{
    return strtol(buffer.text,NULL, 10); // car strtol ne va pas lire \n et apres (car ce n'est pas un chiffre)
}

/**
 * \fn int wordWrapBuffer(FileBuffer buffer)
 * \brief Passe au beginning de la line nexte du buffer donne en parametre
 * \param buffer Buffer lu
*/

int wordWrapBuffer(FileBuffer buffer)
{
    int pos=0;
    unsigned char c = buffer.text[pos];
    pos++;
    while(c!='\n' && pos<buffer.size){
        c = buffer.text[pos];
        pos++;
    }
    return pos;
}

/**
 * \fn void wordWrapFile(FILE* file)
 * \brief Passe au beginning de la line nexte du file donne en parametre
 * \param file file lu
*/

void wordWrapFile(FILE* file)
{
    int c;
    while(((c = fgetc(file))!=EOF) && c!='\n');
}

/**
 * \fn long seekSizeOfFile(FILE* file)
 * \brief Fonction donnant le nombre de caracteres du file lu
 * \param file file lu
 * \return Nombre de caracteres total dans le file
*/

int seekSizeOfFile(FILE* file)
{
    long size=0;
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}
