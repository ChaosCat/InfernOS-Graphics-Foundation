#version 440 core
in vec2 texCoord;
in vec3 colA;

out vec4 color;

uniform sampler2D textureIn;
uniform sampler2D textureIn2;
uniform float mixAccelerator;

void main()
{
	/*if (texCoord.y <= 1.2f && texCoord.y >= 0.8f
		&& texCoord.x >= 0.8f && texCoord.x <= 1.2f)
	{*/
		color = mix(texture(textureIn, texCoord), 
					texture(textureIn2, vec2(1.0f - texCoord.s, texCoord.t)),
					mixAccelerator) * vec4(colA, 1.0f);
	//}
	else { color = vec4(0.0f, 0.0f, 0.0f, 1.0f); }
}
