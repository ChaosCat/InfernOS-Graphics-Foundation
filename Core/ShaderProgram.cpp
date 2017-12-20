#include "../Dependencies/glad/glad.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "ShaderProgram.h"

const GLchar* ShaderProgramErrors::ERR_SHADER_VERTEX_FILE_PATH_INVALID = "ERROR::FILE::SHADER::VERTEX::INVALID_PATH";
const GLchar* ShaderProgramErrors::ERR_SHADER_FRAGMENT_FILE_PATH_INVALID = "ERROR::FILE::SHADER::FRAGMENT::INVALID_PATH";
const GLchar* ShaderProgramErrors::ERR_SHADER_VERTEX_COMPILATION_FAILURE = "ERROR::SHADER::VERTEX::COMPILATION_FAILURE";
const GLchar* ShaderProgramErrors::ERR_SHADER_FRAGMENT_COMPILATION_FAILURE = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE";
const GLchar* ShaderProgramErrors::ERR_SHADER_PROGRAM_LINKING_FAILURE = "ERROR::SHADER::PROGRAM::LINKING_FAILURE";
const GLchar* ShaderProgramErrors::ERR_VALIDATE_COMPONENT_SELECTION = "ERROR::VALIDATE::COMPONENT::INVALID_SELECTION";
const GLchar* ShaderProgramErrors::ERR_VALIDATE_PCOMPONENT_INVALID = "ERROR::VALIDATE::PROGRAM::INVALID_TYPE_SELECTION";
const GLchar* ShaderProgramErrors::ERR_VALIDATE_SCOMPONENT_INVALID = "ERROR::VALIDATE::SHADER::INVALID_TYPE_SELECTION";
const GLchar* ShaderProgramErrors::ERR_LOAD_UNIFORM_TYPE_SELECTION = "ERROR::UNIFORM::LOAD::INVALID_TYPE_SELECTION";
const GLchar* ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION = "ERROR::UNIFORM::LOAD::INVALID_SIZE_SELECTION_FOR_GIVEN_TYPE";
const GLint ShaderProgramErrors::INFO_LOG_BUF_SIZE = 1024;

enum ShaderProgram::Component
{
	SHADER = 10,
	PROGRAM = 20,
	VERTEX = 100,
	FRAGMENT = 200,
	NONE = 0
};

ShaderProgram::ShaderProgram(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath)
{
	_vertexShaderData = std::string(static_cast<std::stringstream const&>(
		std::stringstream() << std::ifstream(vertexShaderPath).rdbuf()).str());
	if (_vertexShaderData.empty())
		std::cout << ShaderProgramErrors::ERR_SHADER_VERTEX_FILE_PATH_INVALID << std::endl;
	std::cout << _vertexShaderData << std::endl;
	_fragmentShaderData = std::string(static_cast<std::stringstream const&>(
		std::stringstream() << std::ifstream(fragmentShaderPath).rdbuf()).str());
	std::cout << _fragmentShaderData << std::endl;
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(_ID); }

GLvoid ShaderProgram::process()
{
	GLuint vertexShader, fragmentShader;
	const GLchar* vertexShaderData = _vertexShaderData.c_str();
	const GLchar* fragmentShaderData = _fragmentShaderData.c_str();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderData, nullptr);
	glCompileShader(vertexShader);
	validate(vertexShader, Component::SHADER, Component::VERTEX);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderData, nullptr);
	glCompileShader(fragmentShader);
	validate(fragmentShader, Component::SHADER, Component::FRAGMENT);
	_ID = glCreateProgram();
	glAttachShader(_ID, vertexShader);
	glAttachShader(_ID, fragmentShader);
	glLinkProgram(_ID);
	validate(_ID, Component::PROGRAM, Component::NONE);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLvoid ShaderProgram::validate(GLuint target, Component component, Component type)
{
	GLint success;
	GLchar infoLog[ShaderProgramErrors::INFO_LOG_BUF_SIZE];
	switch (component)
	{
	case Component::PROGRAM:
		if (type == Component::NONE)
		{
			glGetProgramiv(target, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(target, ShaderProgramErrors::INFO_LOG_BUF_SIZE, nullptr, infoLog);
				std::cout << ShaderProgramErrors::ERR_SHADER_PROGRAM_LINKING_FAILURE << '\n' << infoLog << std::endl;
			}
		}
		else
			std::cout << ShaderProgramErrors::ERR_VALIDATE_PCOMPONENT_INVALID << std::endl;
		break;
	case Component::SHADER:
		switch (type)
		{
		case Component::VERTEX:
			glGetShaderiv(target, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(target, ShaderProgramErrors::INFO_LOG_BUF_SIZE, nullptr, infoLog);
				std::cout << ShaderProgramErrors::ERR_SHADER_VERTEX_COMPILATION_FAILURE << '\n' << infoLog << std::endl;
			}
			break;
		case Component::FRAGMENT:
			glGetShaderiv(target, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(target, ShaderProgramErrors::INFO_LOG_BUF_SIZE, nullptr, infoLog);
				std::cout << ShaderProgramErrors::ERR_SHADER_FRAGMENT_COMPILATION_FAILURE << '\n' << infoLog << std::endl;
			}
			break;
		default:
			std::cout << ShaderProgramErrors::ERR_VALIDATE_SCOMPONENT_INVALID << std::endl;
			break;
		}
		break;
	default:
		std::cout << ShaderProgramErrors::ERR_VALIDATE_COMPONENT_SELECTION << std::endl;
		break;
	}
}

GLvoid ShaderProgram::useProgram() { glUseProgram(_ID); }

template <typename T>
GLvoid ShaderProgram::loadUniform(const GLchar* name, UniformType type, GLuint size, T data0, T data1, T data2, T data3)
{
	switch (type)
	{
	case UniformType::DOUBLE:
		size == 1 ? glUniform1d(glGetUniformLocation(_ID, name),
				static_cast<GLdouble>(data0)) :
			size == 2 ? glUniform2d(glGetUniformLocation(_ID, name),
				static_cast<GLdouble>(data0),
				static_cast<GLdouble>(data1)) :
			size == 3 ? glUniform3d(glGetUniformLocation(_ID, name),
				static_cast<GLdouble>(data0),
				static_cast<GLdouble>(data1),
				static_cast<GLdouble>(data2)) :
			size == 4 ? glUniform4d(glGetUniformLocation(_ID, name),
				static_cast<GLdouble>(data0),
				static_cast<GLdouble>(data1),
				static_cast<GLdouble>(data2),
				static_cast<GLdouble>(data3)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case UniformType::FLOAT:
		size == 1 ? glUniform1f(glGetUniformLocation(_ID, name),
				static_cast<GLfloat>(data0)) :
			size == 2 ? glUniform2f(glGetUniformLocation(_ID, name),
				static_cast<GLfloat>(data0),
				static_cast<GLfloat>(data1)) :
			size == 3 ? glUniform3f(glGetUniformLocation(_ID, name),
				static_cast<GLfloat>(data0),
				static_cast<GLfloat>(data1),
				static_cast<GLfloat>(data2)) :
			size == 4 ? glUniform4f(glGetUniformLocation(_ID, name),
				static_cast<GLfloat>(data0),
				static_cast<GLfloat>(data1),
				static_cast<GLfloat>(data2),
				static_cast<GLfloat>(data3)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case UniformType::INT:
		size == 1 ? glUniform1i(glGetUniformLocation(_ID, name),
				static_cast<GLint>(data0)) :
			size == 2 ? glUniform2i(glGetUniformLocation(_ID, name),
				static_cast<GLint>(data0),
				static_cast<GLint>(data1)) :
			size == 3 ? glUniform3i(glGetUniformLocation(_ID, name),
				static_cast<GLint>(data0),
				static_cast<GLint>(data1),
				static_cast<GLint>(data2)) :
			size == 4 ? glUniform4i(glGetUniformLocation(_ID, name),
				static_cast<GLint>(data0),
				static_cast<GLint>(data1),
				static_cast<GLint>(data2),
				static_cast<GLint>(data3)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case UniformType::UINT:
		size == 1 ? glUniform1ui(glGetUniformLocation(_ID, name),
			static_cast<GLuint>(data0)) :
			size == 2 ? glUniform2ui(glGetUniformLocation(_ID, name),
				static_cast<GLuint>(data0),
				static_cast<GLuint>(data1)) :
			size == 3 ? glUniform3ui(glGetUniformLocation(_ID, name),
				static_cast<GLuint>(data0),
				static_cast<GLuint>(data1),
				static_cast<GLuint>(data2)) :
			size == 4 ? glUniform4ui(glGetUniformLocation(_ID, name),
				static_cast<GLuint>(data0),
				static_cast<GLuint>(data1),
				static_cast<GLuint>(data2),
				static_cast<GLuint>(data3)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	default:
		std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_TYPE_SELECTION << std::endl;
		break;
	}
}
template GLvoid ShaderProgram::loadUniform<GLdouble>(GLchar const*, UniformType, GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
template GLvoid ShaderProgram::loadUniform<GLfloat>(GLchar const*, UniformType, GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
template GLvoid ShaderProgram::loadUniform<GLint>(GLchar const*, UniformType, GLuint, GLint, GLint, GLint, GLint);
template GLvoid ShaderProgram::loadUniform<GLuint>(GLchar const*, UniformType, GLuint, GLuint, GLuint, GLuint, GLuint);

template <typename U>
GLvoid ShaderProgram::loadUniformVector(GLchar const* name, UniformType type, GLuint size, U v)
{	
	switch (type)
	{
	case ShaderProgram::DOUBLE_V:
		size == 1 ? glUniform1dv(glGetUniformLocation(_ID, name), 1, reinterpret_cast<GLdouble*>(v)) :
			size == 2 ? glUniform2dv(glGetUniformLocation(_ID, name), 2, reinterpret_cast<GLdouble*>(v)) :
			size == 3 ? glUniform3dv(glGetUniformLocation(_ID, name), 3, reinterpret_cast<GLdouble*>(v)) :
			size == 4 ? glUniform4dv(glGetUniformLocation(_ID, name), 4, reinterpret_cast<GLdouble*>(v)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case ShaderProgram::FLOAT_V:
		size == 1 ? glUniform1fv(glGetUniformLocation(_ID, name), 1, reinterpret_cast<GLfloat*>(v)) :
			size == 2 ? glUniform2fv(glGetUniformLocation(_ID, name), 2, reinterpret_cast<GLfloat*>(v)) :
			size == 3 ? glUniform3fv(glGetUniformLocation(_ID, name), 3, reinterpret_cast<GLfloat*>(v)) :
			size == 4 ? glUniform4fv(glGetUniformLocation(_ID, name), 4, reinterpret_cast<GLfloat*>(v)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case ShaderProgram::INT_V:
		size == 1 ? glUniform1iv(glGetUniformLocation(_ID, name), 1, reinterpret_cast<GLint*>(v)) :
			size == 2 ? glUniform2iv(glGetUniformLocation(_ID, name), 2, reinterpret_cast<GLint*>(v)) :
			size == 3 ? glUniform3iv(glGetUniformLocation(_ID, name), 3, reinterpret_cast<GLint*>(v)) :
			size == 4 ? glUniform4iv(glGetUniformLocation(_ID, name), 4, reinterpret_cast<GLint*>(v)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	case ShaderProgram::UINT_V:
		size == 1 ? glUniform1uiv(glGetUniformLocation(_ID, name), 1, reinterpret_cast<GLuint*>(v)) :
			size == 2 ? glUniform2uiv(glGetUniformLocation(_ID, name), 2, reinterpret_cast<GLuint*>(v)) :
			size == 3 ? glUniform3uiv(glGetUniformLocation(_ID, name), 3, reinterpret_cast<GLuint*>(v)) :
			size == 4 ? glUniform4uiv(glGetUniformLocation(_ID, name), 4, reinterpret_cast<GLuint*>(v)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	default:
		std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		break;
	}
}
template GLvoid ShaderProgram::loadUniformVector<GLdouble*>(GLchar const*, UniformType, GLuint, GLdouble*);
template GLvoid ShaderProgram::loadUniformVector<GLfloat*>(GLchar const *, UniformType, GLuint, GLfloat*);
template GLvoid ShaderProgram::loadUniformVector<GLint*>(GLchar const*, UniformType, GLuint, GLint*);
template GLvoid ShaderProgram::loadUniformVector<GLuint*>(GLchar const*, UniformType, GLuint, GLuint*);

template <typename M>
GLvoid ShaderProgram::loadUniformMatrix(GLchar const* name, UniformType type, GLuint rows, GLuint cols, GLboolean tps, M m)
{
	switch (type)
	{
	case ShaderProgram::DOUBLE_M:
		if (rows == 2)
			(cols == 2) ? glUniformMatrix2dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 3) ? glUniformMatrix2x3dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 4) ? glUniformMatrix2x4dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		else if (rows == 3)
			(cols == 2) ? glUniformMatrix3x2dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 3) ? glUniformMatrix3dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 4) ? glUniformMatrix3x4dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		else if (rows == 4)
			(cols == 2) ? glUniformMatrix4x2dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 3) ? glUniformMatrix4x3dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			(cols == 4) ? glUniformMatrix4dv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLdouble*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION;
		else std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION;
		break;
	case ShaderProgram::FLOAT_M:
		if (rows == 2)
			(cols == 2) ? glUniformMatrix2fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 3) ? glUniformMatrix2x3fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 4) ? glUniformMatrix2x4fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		else if (rows == 3)
			(cols == 2) ? glUniformMatrix3x2fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 3) ? glUniformMatrix3fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 4) ? glUniformMatrix3x4fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION << std::endl;
		else if (rows == 4)
			(cols == 2) ? glUniformMatrix4x2fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 3) ? glUniformMatrix4x3fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			(cols == 4) ? glUniformMatrix4fv(glGetUniformLocation(_ID, name), 1, tps, reinterpret_cast<GLfloat*>(m)) :
			std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION;
		else std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_SIZE_SELECTION;
		break;
	default:
		std::cout << ShaderProgramErrors::ERR_LOAD_UNIFORM_TYPE_SELECTION;
		break;
	}
}
template GLvoid ShaderProgram::loadUniformMatrix<GLdouble*>(GLchar const*, UniformType, GLuint, GLuint, GLboolean, GLdouble*);
template GLvoid ShaderProgram::loadUniformMatrix<GLfloat*>(GLchar const*, UniformType, GLuint, GLuint, GLboolean, GLfloat*);


