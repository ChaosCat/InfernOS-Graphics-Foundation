#pragma once
#include "../Dependencies/glad/glad.h"
#include "../Dependencies/GLFW/glfw3.h"

namespace HelloWindow {
	GLint appMain();
}

namespace HelloTriangle {
	extern const GLuint CONTEXT_VER_MAJ;
	extern const GLuint CONTEXT_VER_MIN;
	extern const GLuint WINDOW_WIDTH;
	extern const GLuint WINDOW_HEIGHT;
	extern const GLuint VIEWPORT_WIDTH;
	extern const GLuint VIEWPORT_HEIGHT;
	extern const GLchar* TITLE;
	extern const GLchar* ERR_CREATE_WINDOW;
	extern const GLchar* ERR_INIT_GLAD;
	extern const GLchar* ERR_VERTEX_SHADER_COMPILE;
	extern const GLchar* ERR_FRAGMENT_SHADER_COMPILE;
	extern const GLchar* ERR_PROGRAM_LINKING;
	GLvoid processInput(GLFWwindow*);
	GLint appMain();
}

namespace HelloShaders {
	extern const GLchar* vertexShaderSource;
	extern const GLchar* fragmentShaderSource;
	GLvoid processInput(GLFWwindow*);
	GLint appMain();
}

namespace HelloTextures {
	extern const GLchar* vertexShaderSource;
	extern const GLchar* fragmentShaderSource;
	extern GLfloat mixAccelerator;
	GLvoid processInput(GLFWwindow*);
	GLint appMain();
}

namespace HelloTransformations {
	extern const GLchar* vertexShaderSource;
	extern const GLchar* fragmentShaderSource;
	GLvoid processInput(GLFWwindow*);
	GLint appMain();
}