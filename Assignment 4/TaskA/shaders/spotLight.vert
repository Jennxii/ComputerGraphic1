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
uniform vec3 objectColor = vec3(1.0);

out vec3 normal;
out vec3 coneColor;

void main()
{
	coneColor = objectColor;	
	gl_Position = observer.projectionMatrix * observer.viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}