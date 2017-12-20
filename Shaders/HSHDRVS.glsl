#version 330 core
layout (location = 0) in vec3 posA;
layout (location = 1) in vec3 colAIn;

out vec3 colA;

uniform float rightOffset;

void main()
{
	colA = colAIn;
	gl_Position = vec4(posA.x + rightOffset, -posA.y, posA.z, 1.0f);
}
