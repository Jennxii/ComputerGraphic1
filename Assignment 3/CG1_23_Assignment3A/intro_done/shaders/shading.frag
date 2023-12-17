#version 450 core

layout ( location = 0 ) out vec4 fragmentColor;


in vec3 color;
in vec2 uv;


void main()
{	
	fragmentColor = vec4(uv.x, uv.y, 0, 1);
}