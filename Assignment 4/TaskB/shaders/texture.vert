#version 450 core

layout ( location = 0 ) in vec3 vertexPosition;
layout ( location = 1 ) in vec3 vertexNormal;
layout ( location = 2 ) in vec3 vertexColor;
layout ( location = 4 ) in vec2 vertexTexcoordA;
layout ( location = 3 ) in vec2 vertexTexcoordB;


out vec3 color;
out vec2 uvA;


struct Camera
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

uniform Camera observer;		

uniform mat4 modelMatrix = mat4(1.0);
uniform mat3 normalMatrix = mat3(1.0);


out vec3 normal;
out vec4 worldPosition;


void main()
{

	color = vertexColor;
	uvA = vertexTexcoordA;
	//uvA = vertexTexcoordB;

	worldPosition = modelMatrix * vec4(vertexPosition, 1.0);
	
	normal = normalize(normalMatrix * vertexNormal);
	
	gl_Position = observer.projectionMatrix * observer.viewMatrix * worldPosition;
}