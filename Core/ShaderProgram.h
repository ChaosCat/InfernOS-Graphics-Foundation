#pragma once
#include "../Dependencies/glad/glad.h"
#include <string>

namespace ShaderProgramErrors
{

	extern const GLchar* ERR_SHADER_VERTEX_FILE_PATH_INVALID;
	extern const GLchar* ERR_SHADER_FRAGMENT_FILE_PATH_INVALID;
	extern const GLchar* ERR_SHADER_VERTEX_COMPILATION_FAILURE;
	extern const GLchar* ERR_SHADER_FRAGMENT_COMPILATION_FAILURE;
	extern const GLchar* ERR_SHADER_PROGRAM_LINKING_FAILURE;
	extern const GLchar* ERR_VALIDATE_PCOMPONENT_INVALID;
	extern const GLchar* ERR_VALIDATE_SCOMPONENT_INVALID;
	extern const GLchar* ERR_VALIDATE_COMPONENT_SELECTION;
	extern const GLchar* ERR_LOAD_UNIFORM_TYPE_SELECTION;
	extern const GLchar* ERR_LOAD_UNIFORM_SIZE_SELECTION;
	extern const GLint INFO_LOG_BUF_SIZE;
}

class ShaderProgram
{
public:
	enum UniformType : GLushort {
		DOUBLE, 
		DOUBLE_V,
		DOUBLE_M,
		FLOAT,
		FLOAT_V,
		FLOAT_M,
		INT,
		INT_V,
		UINT,
		UINT_V
	};
	ShaderProgram(const GLchar*, const GLchar*);
	~ShaderProgram();
	GLvoid process();
	GLvoid useProgram();
	template <typename T> GLvoid loadUniform(const GLchar*, UniformType, GLuint, T, T, T, T);
	template <typename U> GLvoid loadUniformVector(const GLchar*, UniformType, GLuint, U);
	template <typename M> GLvoid loadUniformMatrix(const GLchar*, UniformType, GLuint, GLuint, GLboolean, M);
	
private:
	enum Component;
	GLuint _ID;
	std::string _vertexShaderData;
	std::string _fragmentShaderData;
	GLvoid validate(GLuint, Component, Component);
};