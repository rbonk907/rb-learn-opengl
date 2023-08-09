#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

/* Global Data */
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	 gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderOrangeSrc = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	 FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderYellowSrc = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	 FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
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
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	check_shader_compile_errors(vertexShader, "VERTEX");

	/* Compile Fragment shader source code */
	unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderOrangeSrc, NULL);
	glCompileShader(fragmentShaderOrange);

	check_shader_compile_errors(fragmentShaderOrange, "FRAGMENT");

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderYellowSrc, NULL);
	glCompileShader(fragmentShaderYellow);

	check_shader_compile_errors(fragmentShaderOrange, "FRAGMENT");
  
	/* Link shaders */
	unsigned int orangeShader, yellowShader;
	orangeShader = glCreateProgram();
	glAttachShader(orangeShader, vertexShader);
	glAttachShader(orangeShader, fragmentShaderOrange);
	glLinkProgram(orangeShader);

	int success;
	char infoLog[512];
	glGetProgramiv(orangeShader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(orangeShader, 512, NULL, infoLog);
		printf("ERROR::LINKER::COMPILATION_FAILED\n");
		printf("%s\n", infoLog);

		exit(1);
	}

	yellowShader = glCreateProgram();
	glAttachShader(yellowShader, vertexShader);
	glAttachShader(yellowShader, fragmentShaderYellow);
	glLinkProgram(yellowShader);
	
	glGetProgramiv(orangeShader, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(yellowShader, 512, NULL, infoLog);
		printf("ERROR::LINKER::COMPILATION_FAILED\n");
		printf("%s\n", infoLog);

		exit(1);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);


	/* set up vertex data (and buffer(s)) and configure vertex attributes */
	/* ------------------------------------------------------------------ */
	float leftTriangle[] = {
    -0.5f,  0.5f, 0.0f,  // top
		 0.0f, -0.5f, 0.0f,  // bottom right
		-1.0f, -0.5f, 0.0f   // bottom left
	};
  float rightTriangle[] = {
    0.5f,  0.5f, 0.0f,  // top
		1.0f, -0.5f, 0.0f,  // bottom right
		0.0f, -0.5f, 0.0f   // bottom left
	};

	unsigned int VBOs[2], VAOs[2];
  glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	/* first triangle setup */
	/* -------------------- */
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangle), leftTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* second triangle setup */
	/* --------------------- */
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangle), rightTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
  /* unbind */
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

    glUseProgram(orangeShader);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(yellowShader);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
  
	/* de-allocate all resources, we don't need them anymore */
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(orangeShader);
	glDeleteProgram(yellowShader);
	
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