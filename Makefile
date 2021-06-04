HEAD = $(wildcard ./include/*.h)
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

all: huffman 

huffman: $(OBJ)
	gcc $^ -o $@

obj/%.o: src/%.c $(HEAD)
	gcc -c $< -o $@

.PHONY : cleanlinux cleanwin doc run

cleanlinux:
	rm obj/*.o

cleanwin:
	del obj\*.o

doc:
	doxygen doxygen/doxyfile

run:
	./huffman