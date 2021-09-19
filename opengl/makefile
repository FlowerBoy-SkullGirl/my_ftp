objects =  main.o
library = -lGL, -lGLEW, -lGLU, -lGL, -lglfw3, -lm, -lXrandr, -lXi, lXxf86vm, -lpthread
LIBRARY_PATH=/usr/lib

testgl: $(objects) 
	gcc -o testgl $(objects) -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread
