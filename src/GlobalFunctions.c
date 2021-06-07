/**
 * \file GlobalFunctions.c
 * \brief Contains functions used multiple times by different files
 * \author Robin Meneust
 * \date 2021
 */

#include "../include/Structures_Define.h"
#include "../include/HuffmanFunctions.h"
 


/**
 * \fn void getFileName(char * fileName)
 * \brief Asks the user to give a name for a file
 * \param fileName Given name
 */

void getFileName(char * fileName)
{
    char* posEndInput = NULL;
    printf("\nFull name of the file that has to be compressed/decompressed (with its extension if it has one) : ");
    if(fgets(fileName, FILENAME_MAX, stdin)==NULL){
        fprintf(stderr, "ERROR : Issue encountered while getting the name of the file");
        exit(EXIT_FAILURE);
    }
    posEndInput = strchr(fileName, '\n'); // We seek the position of the character \n to delete it from the name of the file by replacing it with '\0'
    if(posEndInput!=NULL){
        *posEndInput='\0';
    }
}

/**
 * \fn void bufferToFile(FileBuffer buffer,FILE* file)
 * \brief Writes the content of the buffer in the file
 * \param buffer Buffer read
 * \param file File in which is written the content of the buffer
 */

void bufferToFile(FileBuffer buffer, FILE* file)
{
    rewind(file);
    for(int i=0; i<buffer.size; i++)
    {
        fputc(buffer.text[i], file);
    }
}



/**
 * \fn FileBuffer getPortionOfFileToBuffer(FILE* fileTable, int sizeBuff)
 * \brief Reads a file without starting at the beginning and continue until it has read a certain amount (sizeBuff) of characters. Then it save it in a buffer that is returned
 * \param file File from which we get the buffer
 * \param sizeBuff Size of the return buffer. Number of characters read from the file
 * \return Buffer read from the file
 */


FileBuffer getPortionOfFileToBuffer(FILE* file, int sizeBuff)
{
    FileBuffer bufferChar;
    int c=0;
    int posBuffer=0;
    bufferChar.size = sizeBuff;
    bufferChar.text = (unsigned char*) malloc(bufferChar.size*sizeof(unsigned char));

    c=fgetc(file);
    while(posBuffer<bufferChar.size && c!=EOF){
        bufferChar.text[posBuffer]=c;
        posBuffer++;
        c=fgetc(file);
    }

    return bufferChar;
}





/**
 * \fn FileBuffer fileToBuffer(FILE* file)
 * \brief Writes a file in a buffer
 * \param file file read
 * \return Buffer in which is written the content of the file
 */

FileBuffer fileToBuffer(FILE* file)
{
    FileBuffer buffer;
    buffer.size=seekSizeOfFile(file);
    buffer.text=malloc(buffer.size*sizeof(unsigned char));
    TESTALLOC(buffer.text);
    rewind(file);
    for(int i=0; i<buffer.size; i++)
    {
        buffer.text[i]=fgetc(file);
    }
    return buffer;
}

/**
 * \fn long readNumberLine(FILE* file, long line)
 * \brief Gives the number contained in the given line of the file read (integer value)
 * \param file file read
 * \param line Line of the file that is read
 * \return Value contained in the line read
 */

long readNumberLine(FILE* file, long line)
{
    int c;
    long valeur=0;
    long nbReadLines=0;

    rewind(file);

    while(nbReadLines<line){
        wordWrapFile(file);
        nbReadLines++;
    }

    while(((c=fgetc(file))!=EOF) && c!='\n'){
        valeur=(valeur*10)+c-'0';
    }
    return valeur;
}

/**
 * \fn void wordWrapBuffer(FileBuffer buffer, int* posIn)
 * \brief Goes to the beginning of the next line in the given buffer and keep a track of the position in the buffer (posIn)
 * \param buffer Buffer read
 * \param posIn Used to read the buffer and to keep a track of the reading position so that the calling function has access to the new position
 */

void wordWrapBuffer(FileBuffer buffer, int* posIn)
{
    unsigned char c = buffer.text[*posIn];
    (*posIn)++;
    while(c!='\n' && (*posIn)<buffer.size){
        c = buffer.text[*posIn];
        (*posIn)++;
    }
    
}

/**
 * \fn void wordWrapFile(FILE* file)
 * \brief Goes to the beginning of the next line in the given file
 * \param file File read
 */

void wordWrapFile(FILE* file)
{
    int c;
    while(((c = fgetc(file))!=EOF) && c!='\n');
}

/**
 * \fn long seekSizeOfFile(FILE* file)
 * \brief Gives the number of characters contained in the read file (its size)
 * \param file File read
 * \return Size of the file read
 */

int seekSizeOfFile(FILE* file)
{
    long size=0;
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

/**
 * \fn HuffmanTreeNode* createNodeHuff(unsigned char c, HuffmanTreeNode* leftNode, HuffmanTreeNode* rightNode, HuffmanTreeNode* parentNode)
 * \brief Create a node of type HuffmanTreeNode for a binary tree
 * \param c Character that will be contained in the created node
 * \param leftNode Left node pointed by the created node
 * \param rightNode Right node pointed by the created node
 * \param parentNode Parent node pointed by the created node
 * \return Newly created node of type HuffmanTreeNode
 */

HuffmanTreeNode* createNodeHuff(unsigned char c, HuffmanTreeNode* leftNode, HuffmanTreeNode* rightNode, HuffmanTreeNode* parentNode)
{
    HuffmanTreeNode* new_node = (HuffmanTreeNode*) malloc(sizeof(HuffmanTreeNode));
    TESTALLOC(new_node);
    new_node->c=c;
    new_node->left=leftNode;
    new_node->right=rightNode;
    new_node->parent=parentNode;
    return new_node;
}


