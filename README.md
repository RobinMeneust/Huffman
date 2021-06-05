# Huffman Coding
Project written in C

Zip and unzip files using Huffman coding with Burrows Wheeler and Move To Front



## AUTHOR
Robin MENEUST



## INSTALLATION
Requires the make function for compilation



## COMPILATION
Use the `make` command in the console



## EXECUTION
Directly open the created executable or use the command :
````
make run
````
Then follow the instructions displayed on the screen: give the name of the file with its path

It's also possible to directly drag the file to be compressed onto the executable



## MISCELLANEOUS

* The file table.txt associated with the compressed file is required for the decompression, it must be in the same folder as the executable.
* The file to be compressed must not have the name "table.txt".
* An empty file will not be compressed.
* If there are few identical characters in the file to be compressed (or only one character) the compression will be inefficient.
* To generate the doxygen documentation do :

````
make doc
````

### To remove the .o files you can do the following

#### Windows
````
make cleanwin
````

#### Linux
````
make cleanlinux
````
