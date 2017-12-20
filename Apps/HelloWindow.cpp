#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"
#include <iostream>
#include "apps.h"

using namespace HelloWindow;

GLint HelloWindow::appMain()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, 800, 600);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f / rand(), 1.0f / rand(), 1.0f / rand(), 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
