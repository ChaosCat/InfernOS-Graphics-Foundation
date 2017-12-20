#pragma once
#include "../Dependencies/glad/glad.h"

class TextureParameterQueue
{
public:
	TextureParameterQueue();
	~TextureParameterQueue();

private:
	GLuint _size;
};