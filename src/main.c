/**
 * \file main.c
 * \brief Main function of the project. Used to launch the compression or the decompression
 * \author Robin Meneust
 * \date 2021
 */


#include "../include/HuffmanFunctions.h"
#include "../include/Structures_Define.h"

/**
 * \fn int main(int argc, char *argv[])
 * \brief Main function used to compress or decompress a file given in parameter, and if there isn't one then we ask the user to enter its name
 */

int main(int argc, char *argv[])
{
    char fileNameIn[FILENAME_MAX];
    int choice=0;

    //Choice between compression, decompression and stoping the program
    printf("MENU\n\n");
    printf("0 : Exit\n");
    printf("1 : Compress a file\n");
    printf("2 : Decompress a file\n\n");
    printf("Give a number between 0 and 2 that correponds to your choice : ");
    choice = fgetc(stdin)-'0';
    while(choice<0 || choice>2){
        printf("\nIncorrect choice\nGive a number between 0 and 2 only : ");
        choice = fgetc(stdin)-'0';
    }
    getchar();
    printf("\nchoice %d :\n", choice);

    if(choice!=0){
        //Application of the chosen function
        switch(choice){
            case 1 :
                // Getting the name of the file that we will open
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// If the size of the string is correct to get copied in fileNameIn and if there are only 2 arguments (the executable and the name of the file dropped or given)
                    sprintf(fileNameIn, "%s", argv[1]);
                else{
                    getFileName(fileNameIn);
                }

                if(!strcmp(fileNameIn, "table.txt")){ // The input file can't have the same name of other files used in this program
                    fprintf(stderr, "ERROR : Le nom du file doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                compressMain(fileNameIn);
                break;
            case 2 :
                // Getting the name of the file that we will open
                if(argc==2 && (strlen(argv[1])<FILENAME_MAX))// If the size of the string is correct to get copied in fileNameIn and if there are only 2 arguments (the executable and the name of the file dropped or given)
                    sprintf(fileNameIn, "%s", argv[1]);
                else{
                    getFileName(fileNameIn);
                }

                if(!strcmp(fileNameIn, "table.txt")){ // The input file can't have the same name of other files used in this program
                    fprintf(stderr, "ERROR : Le nom du file doit etre different de table.txt");
                    exit(EXIT_FAILURE);
                }
                decompressMain(fileNameIn);
                break;
            default :
                fprintf(stderr, "ERROR : Incorrect choice");
                exit(EXIT_FAILURE);
        }
    }
    return 0;
}
