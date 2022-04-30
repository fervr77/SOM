#version 460 core

out vec4 fragColor;
uniform vec3 objectColor;

void main(void)
{
	fragColor = vec4(objectColor,1.0f);
}