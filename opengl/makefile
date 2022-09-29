objects =  main.o
library = -lGL, -lGLEW, -lGLU, -lGL, -lglfw3, -lm, -lXrandr, -lXi, lXxf86vm, -lpthread
LIBRARY_PATH=/usr/lib

testgl: $(objects) 
	gcc -o testgl $(objects) -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g

refactor: $(objects) 
	gcc -o refactor refactor.c -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g

refactor2:
	g++ -o refactor refactor.c shaders/shader.cpp -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g

shader:
	g++ -o shaders/shader.o shaders/shader.cpp -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g 

learnopengl:
	g++ -o learngl learnopengl.cpp -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g
testval:
	g++ -o testval testfloatval.cpp -lGL -lGLEW -lGLU -lGL -lglfw -lm -lXrandr -lXi -lXxf86vm -lpthread -g
