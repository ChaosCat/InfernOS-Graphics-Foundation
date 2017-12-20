#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"
#include "../Dependencies/stb_image.h"
#include <iostream>
#include "apps.h"
#include "../Core/ShaderProgram.h"

using namespace HelloTextures;
GLfloat HelloTextures::mixAccelerator = 0;

const GLchar* HelloTextures::vertexShaderSource = \
R"(#version 440 core
layout (location = 0) in vec3 posA;
layout (location = 1) in vec3 colAIn;
layout (location = 2) in vec2 texCoordA;

out vec3 colA;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(posA, 1.0f);
	colA = colAIn;
	texCoord = texCoordA;
}
)";

const GLchar* HelloTextures::fragmentShaderSource = \
R"(#version 430 core
in vec2 texCoord;
in vec3 colA;

out vec4 color;

uniform sampler2D textureIn;
uniform sampler2D textureIn2;
uniform float mixAccelerator;

void main()
{
	if (texCoord.y <= 1.2f && texCoord.y >= 0.8f
		&& texCoord.x >= 0.8f && texCoord.x <= 1.2f)
	{
		color = mix(texture(textureIn, texCoord), 
					texture(textureIn2, vec2(1.0f - texCoord.s, texCoord.t)),
					mixAccelerator) * vec4(colA, 1.0f);
	}
	else { color = vec4(0.0f, 0.0f, 0.0f, 1.0f); }
}
)";

GLvoid HelloTextures::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) mixAccelerator += 0.001f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) mixAccelerator -= 0.001f;
	mixAccelerator = mixAccelerator > 1.0f ? 1.0f : (mixAccelerator < 0.0f) ? 0.0f : mixAccelerator;
}

GLint HelloTextures::appMain()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLint monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	GLFWwindow* window;
	if (monitorCount > 1)
		window = glfwCreateWindow(glfwGetVideoMode(monitors[1])->width,
			glfwGetVideoMode(monitors[1])->height, "Hello Textures", monitors[1], nullptr);
	else window = glfwCreateWindow(800, 600, "Hello Textures", nullptr, nullptr);
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
	if (monitorCount > 1) glViewport(0, 0,
		glfwGetVideoMode(monitors[1])->width, glfwGetVideoMode(monitors[1])->height);
	else glViewport(0, 0, 800, 600);

	ShaderProgram shaderProgram{ "HTEXVS.glsl", "HTEXFS.glsl" };
	shaderProgram.process();
	
	stbi_set_flip_vertically_on_load(GL_TRUE);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint texWidth, texHeight, nrChannels;
	GLubyte* data = stbi_load("resources/box.jpg", &texWidth, &texHeight, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else { std::cout << "ERROR::TEXTURE::LOADING_FAILURE" << std::endl; }
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("resources/star.jpg", &texWidth, &texHeight, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else { std::cout << "ERROR::TEXTURE::LOADING_FAILURE" << std::endl; }
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f
	};
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	
	shaderProgram.useProgram();
	shaderProgram.loadUniform("textureIn",  ShaderProgram::UniformType::INT, 1, 0, 0, 0, 0);
	shaderProgram.loadUniform("textureIn2", ShaderProgram::UniformType::INT, 1, 1, 0, 0, 0);
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderProgram.useProgram();
		shaderProgram.loadUniform("mixAccelerator", ShaderProgram::UniformType::FLOAT, 1, mixAccelerator, 0.0f, 0.0f, 0.0f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}
