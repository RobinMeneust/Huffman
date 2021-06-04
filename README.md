# Codage Huffman
Projet codé en C

Compresse ou décompresse des fichiers en utilisant le codage Huffman

## AUTEURS
Robin MENEUST

## INSTALLATION
Nécessite la fonction make pour la compilation

## COMPILATION
Utiliser dans la console la commande `make`


## EXECUTION
Directement ouvrir l'executable créé ou utiliser la commande :
````
make run
````
Puis suivre les instructions s'affichant à l'écran : donner le nom du fichier avec son chemin...

Ou glisser directement le fichier à compresser sur l'executable


## AUTRE

Le fichier table.txt associé au fichier commpressé est nécessaire pour la décompression, il doit être dans le même dossier que l'exécutable.
Le fichier à compresser ne doit pas avoir pour nom "table.txt".
Un fichier vide ne sera pas compressé.
S'il y a peu de caractères identiques dans le fichier à compresser (ou un seul caractère) la compression sera peu efficace.
Pour générer la documentation doxygen faites :

````
make doc
````

### SUPRESSION DES .o

#### WINDOWS
Pour supprimer les .o sous Windows :
````
make cleanwin
````

#### LINUX
Pour supprimer les .o sous Linux :
````
make cleanlinux
````