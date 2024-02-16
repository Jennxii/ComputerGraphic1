#version 450 core

layout ( location = 0 ) in vec3 vertexPosition;
layout ( location = 1 ) in vec3 vertexNormal;

struct Camera
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

uniform Camera observer;		

uniform mat4 modelMatrix = mat4(1.0);
uniform mat3 normalMatrix = mat3(1.0);

out vec3 normal;

void main()
{
	normal = normalize(normalMatrix * vertexNormal);
	gl_Position = vec4(vertexPosition, 3.14159265);	// i like PI
}