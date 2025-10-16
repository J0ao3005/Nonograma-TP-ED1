all: tp.o domino.o
	@gcc tp.o domino.o -o exe -lm
	@rm -r tp.o domino.o
tp.o: tp.c
	@gcc tp.c -c -Wall
domino.o: domino.c
	@gcc domino.c -c -Wall
run:
	@./exe
val:
	@gcc -g -o exeval domino.c tp.c -Wall
valrun: val
	@valgrind --leak-check=full --track-origins=yes -s ./exeval

