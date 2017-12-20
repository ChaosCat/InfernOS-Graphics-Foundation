#pragma once
#include "../Dependencies/glad/glad.h"

template <typename T>
class TextureParameter
{
public:
	TextureParameter(GLenum target, T value) : _target(target), _value(value) {};
	GLvoid operator=(GLenum target, T value) { _target = target; _value = value; };
	GLvoid operator(GLenum target, T value) { _target = target; _value = value; };
	GLenum target() { return _target; }
	T value() { return _value; }

	TextureParameter target() {};
	~TextureParameter();

private:
	GLenum _target;
	T _value;
};
