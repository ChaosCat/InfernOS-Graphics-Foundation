#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"
#include "../Dependencies/glm/glm.hpp"
#include "../Dependencies/glm/gtc/matrix_transform.hpp"
#include "../Dependencies/glm/gtc/type_ptr.hpp"
#include "../Core/MonitorManager.h"
#include <iostream>
#include "apps.h"

using namespace HelloTransformations;

GLvoid handleInput(GLFWwindow*);

const GLchar* HelloTransformations::vertexShaderSource = \
R"(#version 460 core
layout (location = 0 ) vec3 posA;
layout (location = 1 ) vec2 texCoordA;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(posA, 1);
	texCoord = texCoordA;
}
)";

const GLchar* HelloTransformations::fragmentShaderSource = \
R"(#version 460 core
in vec2 texCord;

out vec4 color;
void main()
{
	color = vec4(1, 1, 1, 1);
}
)";

GLint HelloTransformations::appMain()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	MonitorManager mm;
	GLFWwindow* window = glfwCreateWindow(mm.getActiveMonitorWidth(), mm.getActiveMonitorHeight(), "Transformations", mm.getActiveMonitor(), nullptr);
	if (window == nullptr)
	{
		std::cout << "ERROR::GLFW::INTIALIZATIION_FAILURE\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)gladLoadGLLoader))
	{
		std::cout << "ERROR::GLAD::LOADING_FAILURE\n";
		glfwTerminate();
		return -2;
	}
	glViewport(0, 0, mm.getActiveMonitorWidth(), mm.getActiveMonitorHeight());

	GLuint vertexShader, fragmentShader, shaderProgram;
	GLint success; GLchar log[512];
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE\n" << log << std::endl;
	}
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
		std::cout << "ERROR::SHADER::FRAGMNET::COMPILATION_FAILURE\n" << log << std::endl;
	}
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, log);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILURE\n" << log << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		handleInput(window);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

GLvoid handleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
}
