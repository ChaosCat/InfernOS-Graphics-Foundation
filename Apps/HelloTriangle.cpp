#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"
#include <iostream>
#include "apps.h"
#include "../Core/ShaderProgram.h"
#include "../Core/MonitorManager.h"

using namespace HelloTriangle;

const GLuint HelloTriangle::CONTEXT_VER_MAJ = 3;
const GLuint HelloTriangle::CONTEXT_VER_MIN = 3;
const GLuint HelloTriangle::WINDOW_WIDTH = 800;
const GLuint HelloTriangle::WINDOW_HEIGHT = 600;
const GLuint HelloTriangle::VIEWPORT_WIDTH = 800;
const GLuint HelloTriangle::VIEWPORT_HEIGHT = 600;
const GLchar* HelloTriangle::TITLE = "Hello Triangle";
const GLchar* HelloTriangle::ERR_CREATE_WINDOW = "ERROR::GLFW::WINDOW_CREATION_FAILURE";
const GLchar* HelloTriangle::ERR_INIT_GLAD = "ERROR::GLAD::GLAD_INITIALIZATION_FAILURE";
const GLchar* HelloTriangle::ERR_VERTEX_SHADER_COMPILE = "ERROR::SHADER::VERTEX::COMPILATION_FAILURE";
const GLchar* HelloTriangle::ERR_FRAGMENT_SHADER_COMPILE = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE";
const GLchar* HelloTriangle::ERR_PROGRAM_LINKING = "ERROR::SHADER::PROGRAM::LINKING_FAILURE";

GLvoid HelloTriangle::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLint HelloTriangle::appMain()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CONTEXT_VER_MAJ);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CONTEXT_VER_MIN);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	
	MonitorManager mm;
	mm.setActiveMonitor(1);
	GLFWwindow* window = glfwCreateWindow(mm.getActiveMonitorWidth(),
		mm.getActiveMonitorHeight(), TITLE, 
		mm.getActiveMonitor(), nullptr);
	if (window == nullptr)
	{
		std::cout << ERR_CREATE_WINDOW << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << ERR_INIT_GLAD << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, mm.getActiveMonitorWidth(), mm.getActiveMonitorHeight());

	ShaderProgram shaderProgram1("Shaders/HTVS.glsl", "Shaders/HTFS1.glsl");
	shaderProgram1.process();
	ShaderProgram shaderProgram2("Shaders/HTVS.glsl", "Shaders/HTFS2.glsl");
	shaderProgram2.process();

	GLfloat vertices[] = {
		-0.9f, -0.3f, 0.0f,
		-0.6f, 0.3f, 0.0f,
		-0.3f, -0.3f, 0.0f,
		0.3f, -0.3f, 0.0f,
		0.6f, 0.3f, 0.0f,
		0.9f, -0.3f, 0.0f
	};
	GLuint VAO1, VBO1;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) / 2, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	GLuint VAO2, VBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(9 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram1.useProgram();
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		shaderProgram2.useProgram();
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO1);
	glDeleteBuffers(1, &VBO1);

	glfwTerminate();
	return 0;
}
