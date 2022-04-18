objects = main.o
compiler = cc
libraries = lncurses -lpam -lpam_misc

test: main.c
	g$(compiler) -o test main.c -$(libraries) 

main.o: main.c
	g$(compiler) -o main.o main.c
