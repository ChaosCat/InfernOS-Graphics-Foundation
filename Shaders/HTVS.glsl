#version 330 core
layout (location = 0) in vec3 posA;

void main()
{
	gl_Position = vec4(posA.xyz, 1.0f);
}
