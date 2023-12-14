#version 450 core

layout ( location = 0 ) out vec4 fragmentColor;

in v2f										
{
	vec2 uv;
}
fragment_in;

uniform vec3 diffuseColor = vec3(0.5);
uniform vec2 patternSize = vec2(1.0);
uniform bool usePattern = true;

vec3 pattern(in vec2 coord)
{
	ivec2 c = ivec2(patternSize.x * coord.x, patternSize.y * coord.y);
	if (c.x % 2 == c.y % 2)
		return vec3(1.0);
	else
		return vec3(0.5);
}

void main()
{	
	vec3 patternColor = usePattern ? pattern(fragment_in.uv) : vec3(1.0);
	fragmentColor = vec4(patternColor * diffuseColor, 1.0);
}