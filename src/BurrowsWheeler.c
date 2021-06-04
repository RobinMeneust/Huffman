/**
 * \file BurrowsWheeler.c
 * \brief Apply Burrows Wheeler to the given file (encode) or apply its inverse to get back the initial file (decode)
 * \author Robin Meneust
 * \date 2021
*/

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"
/**
 * \fn unsigned char ** allocateMatBW(long size)
 * \brief Allocate the square matrix of characters given with its size
 * \param size size of the matBW matrix
*/

unsigned char ** allocateMatBW(long size)
{
    unsigned char** mat_return = NULL;
    mat_return = malloc(size*sizeof(unsigned char*));
    TESTALLOC(mat_return);
    for(long i=0; i<size; i++){
        mat_return[i] = malloc(size*sizeof(unsigned char));
        TESTALLOC(mat_return[i]);
    }
    return mat_return;
}

/**
 * \fn void freeMatBW(unsigned char** matBW, long size)
 * \brief Free the given square matrix of characters
 * \param matBW Matrix that we have to free
 * \param size Size of the given matrix
*/

void freeMatBW(unsigned char** matBW, long size)
{
    for(long i=0; i<size; i++){
        free(matBW[i]);
    }
    free(matBW);
}




/**
 * \fn void shiftedFilling(FILE* fileIn, unsigned char** mat, long size, int position)
 * \brief Fill a line of the matrix matBW with all the content of the initial file, it begins with the character at the index "position" and coontinues until all the content as been enter. If we get to the end of the file without filling all the line then we go back to the beginning of the file
 * \param fileIn Initial file that we have to compress
 * \param mat Sorted matrix that we fill
 * \param size Size of matBW
 * \param position Line filled here. Also corresponds to the right shift compared to the initial line (the first one in this matrix)
*/

void shiftedFilling(FILE* fileIn, unsigned char** mat, long size, long position)
{
    unsigned char c;
    long i=position;
    rewind(fileIn);
    c = fgetc(fileIn);
    while(!feof(fileIn)){
        mat[position][i] = c;
        i++;
        if(i==size){
            i=0;
        }
        c=fgetc(fileIn);
    }
    rewind(fileIn);
}

/**
 * \fn long sortMat(char** mat, long size)
 * \brief Sort by ascending value each line of the matrix mat, it considers only the first character of each line, and if those 2 characters are equal then we use the next ones to sort the matrix
 * \param mat Matrix that we are sorting
 * \param size Size of the given matrix
 * \return Index of the line of the sorted matrix that corresponds to the initial text (used only for compression in older versions of this project)
*/

void sortMat(unsigned char** mat, long size)
{
    long i0=0;
    long j=0;
    long k=0;
    int repeat;
    unsigned char* temp = NULL;
    for(long i=1; i<size; i++){
        j=i-1;
        temp = mat[i];
        repeat=1;
        while(j>=0 && repeat && temp[0] <= mat[j][0]){
            if(temp[0] == mat[j][0]){   // si les 2 premieres lettres des lines comparees sont identiques
                k=0;
                while(k<size && temp[k] == mat[j][k]){    // on cherche la 1re lettre differente entre les 2 lines
                    k++;
                }
                if(k<size){   // Si les 2 sont differents
                    if(temp[k]<mat[j][k]){ // Si le text de la line j est encore plus "grand" que celui de la line i (a inserer) par ordre alphabetique
                        mat[j+1] = mat[j];
                        if(j==i0){
                            i0++;
                        }
                        j--;
                    }
                    else{
                        repeat = 0;
                    }
                }
                else{
                    repeat=0;
                }
            }
            else{
                mat[j+1] = mat[j];
                if(j==i0){
                    i0++;
                }
                j--;
            }
        }
        mat[j+1] = temp;
    }
    return i0;
}



/**
 * \fn void saveBWEncode(FILE* fileBW, unsigned char** matBW, long size, long i0)
 * \brief Save the last column of matWB in the file fileBW
 * \param fileBW File that will contain the result of the application of Burrows Wheeler on the initial file
 * \param matBW Sorted square matrix containing in its last column the text that is being saved in fileBW
 * \param size Size of the matrix matBW
 * \param i0 Index written at the beginning of the string, corresponds to the line of matBW containing the content of the initial file
*/

void saveBWEncode(FILE* fileBW, unsigned char** matBW, long size, long i0)
{
    fprintf(fileBW, "%ld\n", i0);
    for(int i=0; i<size; i++){
        fputc(matBW[i][size-1], fileBW);
    }
}


/**
 * \fn void fillColumnBWDecode(FileBuffer bufferBW, unsigned char** matBW, long size, long shift)
 * \brief Decale toutes les colonnes d'une case a droite et remplit la 1re avec le text du file fileBW
 * \param bufferBW Buffer de text genere par cette fonction (c'est le text du fileIn auquel on a applique Burrows Wheeler)
 * \param matBW Matrice carree triee contenant en derniere colonne le text a sauvegarder
 * \param size size de la matrice matBW
 * \param shift Indique le nombre de shifts de colonnes a effectuer(de 0 a size-1)
*/

void fillColumnBWDecode(FileBuffer bufferBW, unsigned char** matBW, long size, long shift)
{
    int pos=0;
    for(long i=0; i<size; i++){
        for(int j=shift; j>0; j--){
            matBW[i][j]=matBW[i][j-1];
        }
        matBW[i][0]=bufferBW.text[pos];
        pos++;
    }
}



/**
 * \fn void saveBWDecode(FILE* fileBWDecode, unsigned char** matBW, long size, long i0)
 * \brief Save in a file the text obtained after applying the inverse of Burrows Wheeler (decode)
 * \param fileBWDecode File filled by this function, it's the content of the initial file before applying Burrows Wheeler
 * \param matBW Sorted square matrix containing in its last coluln the text saved here
 * \param size Size of the matrix matBW
 * \param i0 Index written at the beginning of the string, corresponds to the line of matBW containing the content of the initial file
*/

void saveBWDecode(FILE* fileBWDecode, unsigned char** matBW, long size, long i0)
{
    for(int i=0; i<size; i++){
        fputc(matBW[i0][i], fileBWDecode);
        printf("%c", matBW[i0][i]);
    }
    printf("\n");
}


/**
 * \fn void rotationSort(unsigned char* tabChar,int* indexes, int size)
 * \brief Sort the array of indexes to get the encoded text, it reads the array tabChar by reading from the indexes of the array indexes
 * \param tabChar Array containing a string whose cells will be sorted in the array indexes by this function
 * \param indexes Array of the tabChar indexes
 * \param size Size of the arrays tabChar and indexes
*/

void rotationSort(unsigned char* tabChar,int* indexes, int size)
{
    int progress=0;
    for(int i=0;i<size-1;i++)
    {
        int j=i+1;
        int min=i;
        while(j<size)
        {
            int k=0;
            while(tabChar[(indexes[j]+k)%size]==tabChar[(indexes[min]+k)%size] && k<size)
                k++;
            if(tabChar[(indexes[j]+k)%size]<tabChar[(indexes[min]+k)%size])
            {
                min=j;
            }
            j++;
        }
        int tmp = indexes[min];
        indexes[min] = indexes[i];
        indexes[i] = tmp;
        if(progress+5<((i+1)*100)/size) //Display the progress of the current task
        {
            progress=(int)((((i+1)*100)/size)/5)*5;
            printf("%d%%\n",progress);
        }
    }
}


/**
 * \fn int burrowsWheeler(FileBuffer* bufferIn)
 * \brief Apply Burrows Wheeler to bufferIn
 * \param bufferIn Buffer on which is applied Burrows Wheeler
 * \return Index used to decode the text encoded with Burrows Wheeler
*/

int burrowsWheeler(FileBuffer* bufferIn)
{
    int size = bufferIn->size;
    unsigned char* tabChar=malloc(sizeof(unsigned char)*size);
    int* indexes=malloc(sizeof(int)*size);
    for(int i=0;i<size;i++)
    {
        tabChar[i]=bufferIn->text[i];
        indexes[i]=i;
    }
    rotationSort(tabChar,indexes,size);
    int i=0;
    int beginning=0;
    while(i<size)
    {
        if(indexes[i]==0)
        {
            beginning=i;
            break;
        }
        i++;
    }

    for(int i=0;i<size;i++)
    {
        int id = indexes[i]-1;
        if(id==-1)
            id=size-1;
        bufferIn->text[i]=tabChar[id];
    }
    free(tabChar);
    free(indexes);

    return beginning;
}

/**
 * \fn void burrowsWheelerDecode(FileBuffer bufferIn, FILE* fileBWDecode)
 * \brief Apply the inverse of Burrows-Wheeler to bufferIn and save it in fileBWDecode
 * \param indexBW Index used to decode the text encoded with Burrows Wheeler
 * \param bufferInBuffer Buffer on which is applied the inverse of Burrows Wheeler Buffer
 * \param fileBWDecode File in which is saved the result
*/

void burrowsWheelerDecode(int indexBW, FileBuffer bufferIn, FILE* fileBWDecode)
{
    long size = bufferIn.size;
    int progress=0;
    unsigned char ** matBW = allocateMatBW(size);
    for(long i=0; i<size; i++){
        fillColumnBWDecode(bufferIn, matBW, size, i);
        sortMat(matBW, size);
        if(progress+5<((i+1)*100)/size)
        {
            progress=(int)((((i+1)*100)/size)/5)*5; //Display the progress of the current task
            printf("%d%%\n", progress);
        }
    }

    rewind(fileBWDecode);
    saveBWDecode(fileBWDecode, matBW, size, indexBW);
    freeMatBW(matBW, size);
}
