#pragma once
#include "../Dependencies/glad/glad.h"

class Texture2D {
public:
	enum Type {
		INT, INT_V,
		FLOAT, FLOAT_V
	};
	Texture2D(GLchar const*);
	Texture2D(GLchar const*, GLboolean flip);
	template <typename T> GLvoid setParameter(Type, GLenum, T);

private:
	GLuint _ID;
	GLint _width, _height, _nrChannels;
	GLubyte* _data;
};