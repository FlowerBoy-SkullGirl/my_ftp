objects = main.o
compiler = cc
libraries = lncurses

test: main.c
	g$(compiler) -o test main.c -$(libraries) 

main.o: main.c
	g$(compiler) -o main.o main.c
