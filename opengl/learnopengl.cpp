#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define HEIGHT 600
#define WIDTH 800

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed GLFW\n");
		return 1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window;
	window = glfwCreateWindow(WIDTH,HEIGHT, "Triangle", NULL, NULL);

	if (window == NULL){
		fprintf(stderr, "Failed window\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK){
		fprintf(stderr, "GLEW\n");
	return 1;
	}

	glViewport(0,0,800,600);

	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//SHADERS
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (!success)
	{
		return 1;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//TRIANGLES
	float vertices[] = {
		//4 Points
		-0.5f, 0.0f, 0.0f, //left
		0.0f, -0.5f, 0.0f, //bottom
		0.0f, 0.5f, 0.0f, //top
		0.5f, 0.0f, 0.0f, //right
		0.0f, 0.0f, 0.5f, //front
		0.0f, 0.0f, -0.5f //back
	};
	
	unsigned int indices[] = {
		0, 5, 2, // top left back
		0, 4, 2, // top left front
		3, 4, 2, // top right front
		3, 5, 2, // top right back
		3, 5, 1, // bottom right back
		3, 4, 1, // bottom right front
		0, 5, 1, // bottom left back
		0, 4, 1 // bottom left front
	};

	float vertices2[] = {
		//4 Points
		-0.75f, 0.5f, 0.0f, //left
		-0.5f, 0.25f, 0.0f, //bottom
		-0.5f, 0.75f, 0.0f, //top
		-0.25f, 0.5f, 0.0f, //right
		-0.5f, 0.5f, 0.25f, //front
		-0.5f, 0.5f, -0.25f //back
	};
	
	unsigned int indices2[] = {
		0, 5, 2, // top left back
		0, 4, 2, // top left front
		3, 4, 2, // top right front
		3, 5, 2, // top right back
		3, 5, 1, // bottom right back
		3, 4, 1, // bottom right front
		0, 5, 1, // bottom left back
		0, 4, 1 // bottom left front
	};

	//VERTEX ARRAYS, BUFFERS
	unsigned int VBO[2], VAO[2];
	unsigned int EBO[2];
	glGenBuffers(2, EBO);

	glGenVertexArrays(2, VAO);

	glGenBuffers(2, VBO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &addressOfAnInt);
	
	//Modify a uniform in a shader
	//int UniformLoc = glGetUniformLocation(shaderProgram, "uniformName");
	//glUseProgram(shaderProgram);
	//glUniform4f(UniformLoc, fval1, fval2, fval3, fval4);

	//0 layout vertex shader, 3 size of attrib vec3, type, normalized, space between vertex attrib, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	//0 layout vertex shader, 3 size of attrib vec3, type, normalized, space between vertex attrib, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//GL_FILL
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//can retrieve time
		//glfwGetTime();
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAO[1]);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();

	return 0;
}
