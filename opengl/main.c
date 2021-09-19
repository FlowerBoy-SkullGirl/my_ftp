#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <stddef.h>
	
	
	//error callback to print errors before or after initialize
	void error_callback(int error, const char *description){
		fprintf(stderr, "%s\n", description);
	}
	
	//Key callback for user input
	static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	
	}
	
	int main(void){
		//sets the error func
		glfwSetErrorCallback(error_callback);

		
		//initializes glfw
		if(!glfwInit()){
			puts("Window failed");
		}	
		
		//Which version to allow and window size/name
		GLFWwindow* window = glfwCreateWindow(640, 480, "testgl", NULL, NULL);
		if(!window){
			puts("Failed window");
		}
		
		int screenWidth, screenHeight;
		glfwGetFramebufferSize( window, &screenWidth, &screenHeight);
		
		//Make a context to use openGL api in
		glfwMakeContextCurrent(window);
		//Extension wrangling
		glewExperimental = GL_TRUE;
		

		//set key callback for input		
		glfwSetKeyCallback(window, key_callback);
		

		//Make a framebuffer swap interval
		glfwSwapInterval(1);
		

		//Make sure glew works
		if(GLEW_OK != glewInit()){}
		
		//The opengl viewing window
		glViewport(0, 0, screenWidth, screenHeight);
		
		double time = glfwGetTime();	
		


		
		//The actual window loop
		while(!glfwWindowShouldClose(window)){
			float ratio;
			int width, height;

			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float) height;
			
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
			glMatrixMode(GL_MODELVIEW);

			glLoadIdentity();
			glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
			glEnd();

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		
		//closing the window
		glfwDestroyWindow(window);
		glfwTerminate();
		return 0;
	}
