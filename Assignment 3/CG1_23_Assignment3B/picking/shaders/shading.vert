#version 450 core

layout ( location = 0 ) in vec3 vertexPosition;
layout ( location = 4 ) in vec2 vertexTexcoord;

out v2f										// data passed from vertex shader to fragment shader (with interpolation)									
{
	vec2 uv;
}
vertex_out;

struct Camera
{
	mat4 world2eye;							// VIEW matrix
	mat4 eye2clip;							// PROJECTION matrix
};

uniform Camera observer;		
uniform mat4 object2world = mat4(1.0);		// MODEL matrix	

void main()
{
	vertex_out.uv = vertexTexcoord;
	
	// apply model-view-projection transformation to get clip space position, from right to left
	gl_Position = observer.eye2clip * observer.world2eye * object2world * vec4(vertexPosition, 1.0);
}