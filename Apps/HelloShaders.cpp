#include "../Dependencies/glad/glad.h"
#include "../Dependencies/glad/glad.h"
#include <iostream>
#include "apps.h"

using namespace HelloShaders;

const GLchar* HelloShaders::vertexShaderSource = \
R"(#version 330 core
layout (location = 0) in vec3 posA;
layout (location = 1) in vec3 colAIn;

out vec3 colA;

uniform float rightOffset;

void main()
{
	colA = colAIn;
	gl_Position = vec4(posA.x + rightOffset, -posA.y, posA.z, 1.0f);
}
)";

const GLchar* HelloShaders::fragmentShaderSource = \
R"(#version 330 core
in vec3 colA;

out vec4 color;

void main()
{
	color = vec4(colA, 1.0f);
}
)";

GLvoid HelloShaders::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

GLint HelloShaders::appMain()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLint monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	std::cout << "Active monitors: " << monitorCount << std::endl;
	GLFWwindow* window;
	if (monitorCount > 1)
		window = glfwCreateWindow(800, 600, "Shaders", monitors[1], nullptr);
	else
		window = glfwCreateWindow(800, 600, "Shaders", monitors[0], nullptr);
	if (window == nullptr)
	{
		std::cout << "ERROR::GLFW::WINDOW_CREATION_FAILURE" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::INITIALIZATION_FAILURE" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);

	GLuint vertexShader, fragmentShader, shaderProgram;
	GLchar infoLog[512]; GLint shaderSuccess;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << infoLog << std::endl;
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE\n" << infoLog << std::endl;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderSuccess);
	if (!shaderSuccess)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLint offsetUniformLocation = glGetUniformLocation(shaderProgram, "rightOffset");
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);

		glUniform1f(offsetUniformLocation, (GLfloat)(sin(glfwGetTime()))/2.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}