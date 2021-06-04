/**
 * \file FonctionsHuffman.h
 * \brief Contient les prototypes des fonctions du projet
 * \author Robin Meneust
 * \date 4 juin 2021
 */



#ifndef FonctionsHuffman
#define FonctionsHuffman
#include "Structures_Define.h"


//FonctionsGlobales.c
void saisieNomFichier(char * nomFichier);
int chercherTailleChaine(char * chaine);
void chargerDansFichier(FileBuffer buffer,FILE* fichier);
FileBuffer chargerDansBuffer(FILE* fichier);
long lireLigneNombre(FILE* fichier, long ligne);
void retourLigneFichier(FILE* fichier);
int retourLigneBuffer(FileBuffer buffer);
int chercherTaille(FILE* fichier);
long chercherTailleDansBuffer(FileBuffer buffer);

//CreationTableHuffman.c
PtrListeCode creerNoeud(unsigned char c);
void ajoutDebListe(PtrListeCode *liste, PtrListeCode noeud);
void initialiserCode(CaseTableHuffman* tableHuffman,int tailleTabHuffman,CaseTabOccurrences* tabOccurrences);
CaseTabOccurrences* remplirTabOccurrences(FileBuffer buffer,int* tailleTabOccurrences);
void rech2Min(int* i_min1, int* i_min2, CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences);
void fusionner(int i_min1, int i_min2, CaseTabOccurrences* tabOccurrences,int* tailleTabOccurrences);
void remplirCodeTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman, CaseTabOccurrences* tabOccurrences, int i_min1, int i_min2);
void libererTableHuffman(CaseTableHuffman* tableHuffman,int tailleTabHuffman);
void libererTabOccurrences(CaseTabOccurrences* tabOccurrences,int tailleTabOccurrences);
void enregistrerTable(int indiceBW, CaseTableHuffman* tableHuffman,int tailleTabHuffman, int tailleFichier, unsigned char option);
void creerTableHuffman(int indiceBW, FileBuffer bufferEntree, unsigned char option);

//Compression.c
void compresser(FileBuffer bufferBW, FILE* fichierSortie);
void compresserMain(char* nomFichierEntree);

//Decompression.c
int initialiserTabElementsPossibles(int* tabElements, FILE* table);
unsigned char rechPremierIndiceNonNul(int* tabElements);
void actualiserTabElementsPossibles(int* tabElements, FILE* table, uint8_t bit, int position, int* nbElements);
void decompresser(FILE* fichierEntree, FileBuffer* bufferSortie, FILE* fichierTable);
void decompresserMain();

//BurrowsWheeler.c
unsigned char ** allouerMatBW(long taille);
void libererMat(unsigned char** matBW, long taille);
void remplissageDecale(FILE* fichierEntree, unsigned char** matBW, long taille, long position);
long triMat(unsigned char** mat, long taille);
void sauvegarderTexteBW(FILE* fichierBW, unsigned char** matBW, long taille, long i0);
void remplissageColonneDecode(FileBuffer bufferBW, unsigned char** matBW, long taille, long decalage);
void sauvegarderTexteBWDecode(FILE* fichierBWDecode, unsigned char** matBW, long taille, long i0);
int burrowsWheeler(FileBuffer* bufferEntree);
void burrowsWheelerDecode(int indiceBW, FileBuffer bufferEntree, FILE* fichierBWDecode);

//MoveToFront.c
int chercherCar(unsigned char tab[], int taille, unsigned char c);
void decalerDebCar(unsigned char tab[],int taille, int indice);
void moveToFrontEncode(FileBuffer *buffer);
void moveToFrontDecode(FileBuffer *buffer);

#endif
