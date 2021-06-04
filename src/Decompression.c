/**
 * \file Decompression.c
 * \brief Decompress the file given by the user by using the previously created coding table table.txt
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"


/**
 * \fn int initializePossibleElementsArray(int* possibleElementsArray, FILE* fileTable)
 * \brief Initialize possibleElementsArray so that only cells asociated to characters that exist in fileTable are equal to 1 (and the others to 0)
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \param fileTable File containing the Huffman table (characters associated to their unique binary code)
 * \return Amount of non-null cells
*/

int initializePossibleElementsArray(int* possibleElementsArray, FILE* fileTable)
{
    int nbElements=0;
    unsigned char c;
    rewind(fileTable);
    wordWrapFile(fileTable);
    wordWrapFile(fileTable);
    wordWrapFile(fileTable);
    c=fgetc(fileTable);
    for(int i=0; i<N_ASCII; i++){
        possibleElementsArray[i]=0;
    }
    while(!feof(fileTable)){
        possibleElementsArray[c]=1;
        nbElements++;
        wordWrapFile(fileTable);
        c=fgetc(fileTable);
    }
    return nbElements;
}

/**
 * \fn unsigned char seekFirstPositiveIndex(int* possibleElementsArray)
 * \brief Seek the first index corresponding to a non-null cell in the given array
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \return First non-null index (which is also a character) of possibleElementsArray
*/


unsigned char seekFirstPositiveIndex(int* possibleElementsArray)
{
    for(unsigned char i=0; i<N_ASCII; i++){
        if(possibleElementsArray[i]==1){
            return i;
        }
    }
    printf("ERROR : Array without any non-null value");
    exit(EXIT_FAILURE);
}

/**
 * \fn refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements)
 * \brief Refresh possibleElementsArray ac fonction du bit et de la position etudies (en comparant les valeurs du file table a ce bit)
 * \param possibleElementsArray Array containing the characters associatied to 1 (if they correspond to the code) or to 0 (in the other case)
 * \param table File containing the Huffman table (characters associated to their unique binary code)
 * \param bit Bit etudie, a comparer a la valeur binaire a la position etudiee dans le file table
 * \param position Position du code etudiee dans le file table, nombre positif, 0 correspond a la 1re valeur du code d'une line et plus ce nombre est grand plus on se deplace a droite
 * \param nbElements Nombre de cases de possibleElementsArray non nulles
*/

void refreshPossibleElementsArray(int* possibleElementsArray, FILE* table, uint8_t bit, int position, int* nbElements)
{
    rewind(table);
    wordWrapFile(table);
    wordWrapFile(table);
    wordWrapFile(table);
    unsigned char c = fgetc(table);
    unsigned char c_table;
    int i=0;

    while(!feof(table)){
        if(possibleElementsArray[c]!=0){
            c_table=fgetc(table);
            i=0;
            while(i<position && c_table!='\n' && !feof(table)){
                c_table=fgetc(table);
                i++;
            }
            if(c_table-'0'!=bit){
                possibleElementsArray[c]=0;
                (*nbElements)--;
            }
        }
        wordWrapFile(table);
        c = fgetc(table);
    }
}



/**
 * \fn void decompress(FILE* fileIn, FILE* bufferOut, FILE* fileTable)
 * \brief Decompresse le fileIn a partir du file table.txt
 * \param fileIn file text initial a decompress
 * \param bufferOut Buffer decompresse genere par cette fonction
 * \param fileTable File containing the Huffman table (characters associated to their unique binary code)
*/


void decompress(FILE* fileIn, FileBuffer* bufferOut, FILE* fileTable)
{
    int sizeFileIn = readNumberLine(fileTable, 2);
    int nbAjouts=0; // nombre de caracteres decompresses
    uint8_t buffer=0;
    uint8_t bit;
    int progress=0;
    int sizeBuffer;
    int position;
    int possibleElementsArrayPossibles[N_ASCII];  // contient 0 ou 1 : 1 = la sequence correspond et 0 sinon
    int nbElementsPossibles = N_ASCII;
    int pos=0;
    bufferOut->text = (unsigned char*) malloc(sizeof(unsigned char)*sizeFileIn);
    bufferOut->size=sizeFileIn;
    rewind(fileIn);
    buffer=fgetc(fileIn);
    position=0;
    sizeBuffer=8;
    nbElementsPossibles = initializePossibleElementsArray(possibleElementsArrayPossibles, fileTable);
    while(nbAjouts<sizeFileIn && nbElementsPossibles!=0){
        while(sizeBuffer!=0 && nbElementsPossibles!=0 && nbAjouts<sizeFileIn){
            bit = buffer & (1<<(sizeBuffer-1));  // on applique un masque pour recuperer le bit a la position sizeBuffer-1 (entre 2^0 et 2^7)
            if(sizeBuffer>1){
                bit >>= sizeBuffer-1;  // on decale vers la droite afin d'avoir la valeur etudiee completement a droite (et donc d'avoir bit valant 0 ou 1)
            }
            refreshPossibleElementsArray(possibleElementsArrayPossibles, fileTable, bit, position, &nbElementsPossibles);
            position++;
            if(nbElementsPossibles==1){ // il ne reste plus qu'une possibilite, c'est donc le bon caractere
                unsigned char c=seekFirstPositiveIndex(possibleElementsArrayPossibles);
                bufferOut->text[pos]=c;
                pos++;
                position=0;
                nbElementsPossibles = initializePossibleElementsArray(possibleElementsArrayPossibles, fileTable);
                nbAjouts++;
            }
            sizeBuffer--;
        }

        if(progress+5<((nbAjouts+1)*100)/sizeFileIn)
        {
            progress=(int)((((nbAjouts+1)*100)/sizeFileIn)/5)*5; //Display the progress of the current task
            printf("%d%%\n", progress);
        }

        buffer=fgetc(fileIn);
        sizeBuffer=8;
    }

    if(nbElementsPossibles==0){
        exit(EXIT_FAILURE);
    }
}


/**
 * \fn void decompressMain(char* fileNameIn)
 * \brief Fonction principale pour la decompression : appelle les fonctions necessaires a la decompression du file "file decompresse.txt" dans le meme repertoire que le programme
 * \param fileNameIn Nom du file a decompress
*/


void decompressMain(char* fileNameIn)
{
    FILE* fileIn;
    FILE* fileOut;
    FILE* fileTable;
    int indexBW;

    fileIn = fopen(fileNameIn, "rb");
    TESTFOPEN(fileIn);
    fileTable = fopen("table.txt", "rb");
    TESTFOPEN(fileTable);

    indexBW=readNumberLine(fileTable, 0);

    printf("\nDecompression du file selon la table...\n");
    FileBuffer buffertext;
    decompress(fileIn, &buffertext, fileTable);
    FCLOSE(fileIn);
    
    int option=readNumberLine(fileTable, 1);
    printf("opt : %d\n", option);
    FCLOSE(fileTable);
    
    int fileSizeNameEntree = seekStringSize(fileNameIn);
    if(fileNameIn[fileSizeNameEntree-4]=='.' && fileNameIn[fileSizeNameEntree-3]=='b' && fileNameIn[fileSizeNameEntree-2]=='i' && fileNameIn[fileSizeNameEntree-1]=='n'){
        fileNameIn[fileSizeNameEntree-4]='\0';  //On enleve le .bin
    }

    //Cas ou le file existe deja
    #if __linux__
    if(access(fileNameIn, F_OK)!=0){
        printf("ERREUR : Le file %s existe deja", fileNameIn);
        printf("Saisissez un nom pour le file decompresse : \n");
        getFileName(fileNameIn);
    }
    #endif
    #if __WIN32__
    if(_access(fileNameIn, 0)!=-1){
        printf("ERREUR : Le file %s existe deja", fileNameIn);
        printf("Saisissez un nom pour le file decompresse : \n");
        getFileName(fileNameIn);
    }
    #endif

    fileOut = fopen(fileNameIn, "wb+"); 
    TESTFOPEN(fileOut);
    if(option==1)
    {
        printf("Application de l'inverse de Move To Front...\n");
        moveToFrontDecode(&buffertext);

        printf("\nApplication de l'inverse de Burrows Wheeler...\n");
        burrowsWheelerDecode(indexBW, buffertext, fileOut);
    }
    else
    {
        bufferToFile(buffertext,fileOut);
    }
    FCLOSE(fileOut);
    free(buffertext.text);
    printf("\nLa decompression est terminee\n");
}
