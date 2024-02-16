#version 450 core

layout ( location = 0 ) out vec4 fragmentColor;

in vec3 coneColor;


void main()
{
	
	fragmentColor = vec4(coneColor, 1.0);
}