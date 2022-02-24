objects = main.o my_string.o 
LIBRARY_PATH=/urs/lib

my_string.o: my_string.cpp my_string.h
	g++ -c my_string.cpp
main.o: main.cpp my_string.cpp my_string.h 
	g++ -c main.cpp 
classes: $(objects)
	g++ -o classes main.o 
