all: tp.o hash.o indiceInvertido.o
	@gcc tp.o hash.o indiceInvertido.o -o exe
	@rm -r hash.o indiceInvertido.o tp.o
tp.o: tp.c
	@gcc tp.c -c -Wall
indiceInvertido.o: indiceInvertido.c
	@gcc indiceInvertido.c -c -Wall
hash.o: hash.c
	@gcc hash.c -c -Wall
run:
	@./exe
val:
	@gcc -g -o exeval hash.c indiceInvertido.c tp.c -Wall
valrun: val
	@valgrind --leak-check=full --track-origins=yes -s ./exeval
