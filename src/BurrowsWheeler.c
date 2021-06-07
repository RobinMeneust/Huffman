/**
 * \file BurrowsWheeler.c
 * \brief Applies Burrows Wheeler to the given file (encode) or apply its inverse to get back the initial file (decode)
 * \author Robin Meneust
 * \date 2021
 */

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"


/**
 * \fn void rotationSort(unsigned char* tabChar, int* indexes, int size)
 * \brief Sorts the array of indexes to get the encoded text, it reads the array tabChar by reading from the indexes of the array indexes
 * \param tabChar Array containing a string whose cells will be sorted in the array indexes by this function
 * \param indexes Array of the tabChar indexes
 * \param size Size of the arrays tabChar and indexes
 */

void rotationSort(unsigned char* tabChar, int* indexes, int size)
{
    int progress=0;
    for(int i=0; i<size-1; i++)
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
        
        if(progress+5 < 100*(((double)i)/size))
        {
            progress+=5; 
            printf("%d%%\n", progress); //Displays the progress of the current task
        }
    }
}


/**
 * \fn bubbleSortIndexes(unsigned char* tabChar, int* indexes, int size)
 * \brief Sorts the array of indexes by using a bubble sort
 * \param tabChar Array containing a string whose cells will be sorted in the array indexes by this function
 * \param indexes Array of the tabChar indexes
 * \param size Size of the arrays tabChar and indexes
 */

void bubbleSortIndexes(unsigned char* tabChar, int* indexes, int size)
{
    int progress=0;
    int i=size-1;
    int temp=0;
    int messy=0;

    do{
        messy = 0;
        for (int j=0; j<i; j++){
            if(tabChar[indexes[j]]>tabChar[indexes[j+1]]){
                messy = 1;
                temp = indexes[j];
                indexes[j] = indexes[j+1];
                indexes[j+1] = temp;
            }
        }
        i--;
    }while(messy && i>0);
}


/**
 * \fn int burrowsWheeler(FileBuffer* bufferIn)
 * \brief Applies Burrows Wheeler to bufferIn
 * \param bufferIn Buffer on which is applied Burrows Wheeler
 * \return Index used to decode the text encoded with Burrows Wheeler
 */

int burrowsWheeler(FileBuffer* bufferIn)
{
    int size = bufferIn->size;
    unsigned char* tabChar=malloc(sizeof(unsigned char)*size);
    int* indexes=malloc(sizeof(int)*size);
    for(int i=0; i<size; i++)
    {
        tabChar[i]=bufferIn->text[i];
        indexes[i]=i;
    }
    rotationSort(tabChar, indexes, size);
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

    for(int j=0; j<size; j++)
    {
        int id = indexes[j]-1;
        if(id==-1)
            id=size-1;
        bufferIn->text[j]=tabChar[id];
    }
    free(tabChar);
    free(indexes);

    return beginning;
}

/**
 * \fn void burrowsWheelerDecode(FileBuffer bufferIn, FILE* fileBWDecode)
 * \brief Applies the inverse of Burrows-Wheeler to bufferIn and save it in fileBWDecode
 * \param indexBW Index used to decode the text encoded with Burrows Wheeler
 * \param bufferInBuffer Buffer on which is applied the inverse of Burrows Wheeler Buffer
 * \param fileBWDecode File in which is saved the result
 */

void burrowsWheelerDecode(int indexBW, FileBuffer bufferIn, FILE* fileBWDecode)
{
    int progress=0;
    int* indexes= (int*) malloc(sizeof(int*)*bufferIn.size); // Will contained sorted indexes
    int i=0;
    int nbW=0; // Number of written characters

    for(int i=0; i<bufferIn.size; i++)
    {
        indexes[i]=i;
    }

    printf("\nSorting indexes for Burrows Wheeler...\n");
    bubbleSortIndexes(bufferIn.text, indexes, bufferIn.size);
    
    rewind(fileBWDecode);
    i = indexes[indexBW];
    printf("\nThe output file is being filled...\n");
    while(nbW<bufferIn.size){
        fputc(bufferIn.text[i], fileBWDecode);
        i=indexes[i];
        nbW++;

        if(progress+5 < 100*(((double)nbW)/bufferIn.size))
        {
            progress+=5; 
            printf("%d%%\n", progress); //Displays the progress of the current task
        }

    }

    free(indexes);
}
