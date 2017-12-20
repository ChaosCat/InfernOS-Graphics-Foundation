#version 440 core
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
