#include "Texture2D.h"
#include "../Dependencies/glad/glad.h"
#include "../Dependencies/stb_image.h"
#include <iostream>

Texture2D::Texture2D(GLchar const* source)
{
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_data = stbi_load(source, &_width, &_height, &_nrChannels, 0);
	if (_data != nullptr)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(GLchar const* source, GLboolean flip)
{
	stbi_set_flip_vertically_on_load(flip);
	Texture2D::Texture2D(source);
}

template <typename T>
GLvoid Texture2D::setParameter(Type type, GLenum target, T value)
{
	type == Texture2D::Type::INT ? glTexParameteri(GL_TEXTURE_2D, target, static_cast<GLint>(value)) :
		type == Texture2D::Type::FLOAT ? glTexParameterf(GL_TEXTURE_2D, target, static_cast<GLfloat>(value)) :
		type == Texture2D::Type::INT_V ? glTexParameteriv(GL_TEXTURE_2D, target, reinterpret_cast<GLint*>(value)) :
		std::cout << "ERROR" << std::endl;
}
template GLvoid Texture2D::setParameter<GLint>(Type, GLenum, GLint);



