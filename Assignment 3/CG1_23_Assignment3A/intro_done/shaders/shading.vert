#version 450 core

layout ( location = 0 ) in vec3 vertexPosition;
layout ( location = 1 ) in vec3 vertexColor;
layout ( location = 2 ) in vec3 vertexUV;


out vec3 color;
out vec2 uv;


struct Camera
{
	mat4 world2eye;							// VIEW matrix
	mat4 eye2clip;							// PROJECTION matrix
};

// you can declare structs and use them as uniforms --> check the C++ code to see how you can set the values
uniform Camera observer;		

uniform mat4 object2world = mat4(1.0);		// MODEL matrix	

void main()
{
	color = vertexColor;
	color = vertexUV;

	
	// apply model-view-projection transformation to get clip space position, from right to left
	gl_Position = observer.eye2clip * observer.world2eye * object2world * vec4(vertexPosition, 1.0);
}