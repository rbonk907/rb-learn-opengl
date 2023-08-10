#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shader.h"

/* Global Data */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"	 gl_Position = vec4(aPos, 1.0);\n"
"  ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	 FragColor = vec4(ourColor, 1.0);\n"
"}\0";

/* Pototypes */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void check_shader_compile_errors(unsigned int shader, const char* shader_type);

/* Functions */
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/* Initialize GLAD to call OpenGL functions */
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
  
	/* Build and compile shader program  */
	/* --------------------------------- */
	/* Compile vertex shader source code */
#if 0
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	check_shader_compile_errors(vertexShader, "VERTEX");

	/* Compile Fragment shader source code */
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	check_shader_compile_errors(fragmentShader, "FRAGMENT");
  
	/* Link shaders */
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR::LINKER::COMPILATION_FAILED\n");
		printf("%s\n", infoLog);

		exit(1);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#endif

	Shader_T ourShader = Shader_new("./shader.vert", "./shader.frag");

	/* set up vertex data (and buffer(s)) and configure vertex attributes */
	/* ------------------------------------------------------------------ */
	float vertices[] = {
		// positions         // colors 
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right red
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left green
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // top blue   
	};
	
	unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  /* position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/* color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// uncomment this call to draw in wireframe polygons
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		
		/* rendering commands go here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

    // glUseProgram(shaderProgram);
		Shader_use(ourShader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
  
	/* de-allocate all resources, we don't need them anymore */
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	Shader_free(ourShader);
	
	glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void check_shader_compile_errors(unsigned int shader, const char* shader_type) 
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  
	if (!success) 
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR::SHADER::%s::COMPILATION_FAILED\n", shader_type);
		printf("%s\n", infoLog);

		exit(1);
	}
}