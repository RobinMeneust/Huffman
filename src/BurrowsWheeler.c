/**
 * \file BurrowsWheeler.c
 * \brief Applique Burrows Wheeler sur le fichier transmis a la fonction, ou bien applique son inverse pour revenir au texte initial.
 * \author Robin Meneust
 * \date 4 juin 2021
 */

#include "../include/Structures_Define.h"
#include "../include/FonctionsHuffman.h"
/**
 * \fn unsigned char ** allouerMatBW(long taille)
 * \brief Alloue la matrice carree de caracteres dont la taille est donnee en parametre
 * \param taille Taille de la matrice matBW
 */

unsigned char ** allouerMatBW(long taille)
{
    unsigned char** mat_retour = NULL;
    mat_retour = malloc(taille*sizeof(unsigned char*));
    TESTALLOC(mat_retour);
    for(long i=0; i<taille; i++){
        mat_retour[i] = malloc(taille*sizeof(unsigned char));
        TESTALLOC(mat_retour[i]);
    }
    return mat_retour;
}

/**
 * \fn void libererMat(unsigned char** matBW, long taille)
 * \brief Libere la matrice carree de caracteres dont la taille est donnee en parametre
 * \param matBW Matrice a liberer
 * \param taille Taille de la matrice matBW
 */

void libererMat(unsigned char** matBW, long taille)
{
    for(long i=0; i<taille; i++){
        free(matBW[i]);
    }
    free(matBW);
}




/**
 * \fn void remplissageDecale(FILE* fichierEntree, unsigned char** matBW, long taille, int position)
 * \brief Remplit une ligne de la matrice matBW avec tout le texte du fichier initial, en partant du caractere a l'indice "position" et en continuant jusqu'a avoir entre tout le texte. Si on arrive a la fin du fichier et qu'on n'a pas rempli la ligne on revient au debut du fichier
 * \param fichierEntree Fichier texte initial a compresser
 * \param matBW Matrice carree triee contenant en derniere colonne le texte a sauvegarder
 * \param taille Taille de la matrice matBW
 * \param position Ligne a remplir. Correpond aussi au decalage vers la droite (indice du 1er caractere lu)
 */

void remplissageDecale(FILE* fichierEntree, unsigned char** matBW, long taille, long position)
{
    unsigned char c;
    long i=position;
    rewind(fichierEntree);
    c = fgetc(fichierEntree);
    while(!feof(fichierEntree)){
        matBW[position][i] = c;
        i++;
        if(i==taille){
            i=0;
        }
        c=fgetc(fichierEntree);
    }
    rewind(fichierEntree);
}

/**
 * \fn long triMat(char** mat, long taille)
 * \brief Trie par ordre alphabetique chaque ligne de la matrice de caracteres donnee en entree en ne considerant que le 1er caractere de chaque ligne, puis si les 2 caracteres sont egaux on considere les suivants
 * \param mat Matrice de caracteres a trier
 * \param taille Taille de la matrice mat
 * \return Indice de la ligne de la matrice matBW correspondant au texte initial
 */

long triMat(unsigned char** mat, long taille)
{
    long i0=0;
    long j=0;
    long k=0;
    int continuer;
    unsigned char* temp = NULL;
    for(long i=1; i<taille; i++){
        j=i-1;
        temp = mat[i];
        continuer=1;
        while(j>=0 && continuer && temp[0] <= mat[j][0]){
            if(temp[0] == mat[j][0]){   // si les 2 premieres lettres des lignes comparees sont identiques
                k=0;
                while(k<taille && temp[k] == mat[j][k]){    // on cherche la 1re lettre differente entre les 2 lignes
                    k++;
                }
                if(k<taille){   // Si les 2 sont differents
                    if(temp[k]<mat[j][k]){ // Si le texte de la ligne j est encore plus "grand" que celui de la ligne i (a inserer) par ordre alphabetique
                        mat[j+1] = mat[j];
                        if(j==i0){
                            i0++;
                        }
                        j--;
                    }
                    else{
                        continuer = 0;
                    }
                }
                else{
                    continuer=0;
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
 * \fn void sauvegarderTexteBW(FILE* fichierBW, unsigned char** matBW, long taille, long i0)
 * \brief Sauvegarde la derniere colonne de la matrice matBW dans le fichier fichierBW
 * \param fichierBW Fichier texte genere par cette fonction (c'est le texte du fichierEntree auquel on applique Burrows Wheeler)
 * \param matBW Matrice carree triee contenant en derniere colonne le texte a sauvegarder
 * \param taille Taille de la matrice matBW
 * \param i0 Indice a ecrire au debut de la chaine, correspond a la ligne de la matrice matBW correspondant au texte initial
 */

void sauvegarderTexteBW(FILE* fichierBW, unsigned char** matBW, long taille, long i0)
{
    fprintf(fichierBW, "%ld\n", i0);
    for(int i=0; i<taille; i++){
        fputc(matBW[i][taille-1], fichierBW);
    }
}


/**
 * \fn void remplissageColonneDecode(FileBuffer bufferBW, unsigned char** matBW, long taille, long decalage)
 * \brief Decale toutes les colonnes d'une case a droite et remplit la 1re avec le texte du fichier fichierBW
 * \param bufferBW Buffer de text genere par cette fonction (c'est le texte du fichierEntree auquel on a applique Burrows Wheeler)
 * \param matBW Matrice carree triee contenant en derniere colonne le texte a sauvegarder
 * \param taille Taille de la matrice matBW
 * \param decalage Indique le nombre de decalages de colonnes a effectuer(de 0 a taille-1)
 */

void remplissageColonneDecode(FileBuffer bufferBW, unsigned char** matBW, long taille, long decalage)
{
    int pos=0;
    for(long i=0; i<taille; i++){
        for(int j=decalage; j>0; j--){
            matBW[i][j]=matBW[i][j-1];
        }
        matBW[i][0]=bufferBW.texte[pos];
        pos++;
    }
}



/**
 * \fn void sauvegarderTexteBWDecode(FILE* fichierBWDecode, unsigned char** matBW, long taille, long i0)
 * \brief Sauvegarde dans un fichier le texte recupere a partir de l'operation inverse de burrows wheeler
 * \param fichierBWDecode Fichier texte genere par cette fonction (c'est le texte initial avant compression)
 * \param matBW Matrice carree triee contenant en derniere colonne le texte a sauvegarder
 * \param taille Taille de la matrice matBW
 * \param i0 Indice correspondant a la ligne de la matrice matBW contenant le texte initial
 */

void sauvegarderTexteBWDecode(FILE* fichierBWDecode, unsigned char** matBW, long taille, long i0)
{
    printf("BW Save:\n");
    for(int i=0; i<taille; i++){
        fputc(matBW[i0][i], fichierBWDecode);
        printf("%c", matBW[i0][i]);
    }
    printf("\n");
}


/**
 * \fn void tri(unsigned char* tabCar,int* indices, int taille)
 * \brief Trie le tableau d'indices afin d'obtenir le texte encode, en lisant le tableau tabCar en lisant a partir des indices du tableau indices
 * \param tabCar Tableau contenant un texte dont les indices seront tries dans le tableau indices
 * \param indices Tableau des indices du tableau tabCar
 * \param taille Taille des tableaux tabCar et indices
 */

void tri(unsigned char* tabCar,int* indices, int taille)
{
    int avancement=0;
    for(int i=0;i<taille-1;i++)
    {
        int j=i+1;
        int min=i;
        while(j<taille)
        {
            int k=0;
            while(tabCar[(indices[j]+k)%taille]==tabCar[(indices[min]+k)%taille] && k<taille)
                k++;
            if(tabCar[(indices[j]+k)%taille]<tabCar[(indices[min]+k)%taille])
            {
                min=j;
            }
            j++;
        }
        int tmp = indices[min];
        indices[min] = indices[i];
        indices[i] = tmp;
        if(avancement+5<((i+1)*100)/taille) //Affiche l'etat d'anvancement de la tache
        {
            avancement=(int)((((i+1)*100)/taille)/5)*5;
            printf("%d%%\n",avancement);
        }
    }
}


/**
 * \fn int burrowsWheeler(FileBuffer* bufferEntree)
 * \brief Applique la methode Burrows-Wheeler sur le fichier fichierEntree et le sauvegarde dans un nouveau fichier
 * \param bufferEntree Buffer de texte genere par la fonction burrowsWheeler (fichier d'entree)
 * \return Indice permettant de decoder le texte BW
*/

int burrowsWheeler(FileBuffer* bufferEntree)
{
    int taille = bufferEntree->taille;
    unsigned char* tabCar=malloc(sizeof(unsigned char)*taille);
    int* indices=malloc(sizeof(int)*taille);
    for(int i=0;i<taille;i++)
    {
        tabCar[i]=bufferEntree->texte[i];
        indices[i]=i;
    }
    tri(tabCar,indices,taille);
    int i=0;
    int debut=0;
    while(i<taille)
    {
        if(indices[i]==0)
        {
            debut=i;
            break;
        }
        i++;
    }

    for(int i=0;i<taille;i++)
    {
        int id = indices[i]-1;
        if(id==-1)
            id=taille-1;
        bufferEntree->texte[i]=tabCar[id];
    }
    free(tabCar);
    free(indices);

    return debut;
}

/**
 * \fn void burrowsWheelerDecode(FileBuffer bufferEntree, FILE* fichierBWDecode)
 * \brief Applique la methode inverse de Burrows-Wheeler sur le fichier fichierBW et le sauvegarde dans un nouveau fichier fichierBWDecode
 * \param indiceBW Indice permettant de decoder le texte BW
 * \param bufferEntree Buffer de texte genere par la fonction burrowsWheeler (fichier d'entree)
 * \param fichierBWDecode Fichier texte genere par cette fonction (c'est le texte initial avant compression) (fichier de sortie)
 */

void burrowsWheelerDecode(int indiceBW, FileBuffer bufferEntree, FILE* fichierBWDecode)
{
    long taille = bufferEntree.taille;
    int avancement=0;
    unsigned char ** matBW = allouerMatBW(taille);
    for(long i=0; i<taille; i++){
        remplissageColonneDecode(bufferEntree, matBW, taille, i);
        triMat(matBW, taille);
        if(avancement+5<((i+1)*100)/taille)
        {
            avancement=(int)((((i+1)*100)/taille)/5)*5; //Affiche l'etat d'anvancement de la tache
            printf("%d%%\n", avancement);
        }
    }

    rewind(fichierBWDecode);
    sauvegarderTexteBWDecode(fichierBWDecode, matBW, taille, indiceBW);
    libererMat(matBW, taille);
}
