# Codage Huffman
Projet écrit en C

Compresse ou décompresse des files en utilisant le codage Huffman

## AUTEUR
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
Puis suivre les instructions s'affichant à l'écran : donner le nom du file avec son chemin...

Il est aussi possible de glisser directement le file à compress sur l'executable


## AUTRE

* Le file table.txt associé au file commpressé est nécessaire pour la décompression, il doit être dans le même dossier que l'exécutable.
* Le file à compress ne doit pas avoir pour nom "table.txt".
* Un file vide ne sera pas compressé.
* S'il y a peu de caractères identiques dans le file à compress (ou un seul caractère) la compression sera peu efficace.
* Pour générer la documentation doxygen faites :

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
