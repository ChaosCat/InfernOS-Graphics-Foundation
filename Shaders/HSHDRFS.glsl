#version 330 core
in vec3 colA;

out vec4 color;

void main()
{
	color = vec4(colA, 1.0f);
}
