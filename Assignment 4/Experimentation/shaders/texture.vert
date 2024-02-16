#version 450 core

layout ( location = 0 ) in vec3 vertexPosition;
layout ( location = 1 ) in vec3 vertexNormal;
layout ( location = 2 ) in vec3 vertexColor;
layout ( location = 4 ) in vec2 vertexTexcoordA;
layout ( location = 3 ) in vec2 vertexTexcoordB;


out vec3 color;
out vec2 uvA;
//out vec2 uvB;

struct Camera
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
};

uniform Camera observer;		

uniform mat4 modelMatrix = mat4(1.0);
uniform mat3 normalMatrix = mat3(1.0);
uniform float phase;

out vec3 normal;
out vec4 worldPosition;

vec3 displace(vec3 pos)
{
    float amplification = 1 - pos.x;
    float amplitude = .06;
    float frequency = 10.0;
    float displacement = amplitude * cos(phase + frequency * pos.x) * amplification ;
    pos = pos + displacement * normal;
	pos.y -= amplification / 2;
    return pos;
}

void main()
{
	
	color = vertexColor;
	uvA = vertexTexcoordA;
	//uvA = vertexTexcoordB;

	worldPosition = modelMatrix * vec4((vertexPosition), 1.0);
	
	normal = normalize(normalMatrix * vertexNormal);
	
	gl_Position = observer.projectionMatrix * observer.viewMatrix * modelMatrix * vec4(displace(vertexPosition), 1);

	vec3 displaced=displace(vertexPosition);
    vec3 displacedRight=displace(vertexPosition+vec3(0.01,0,0));
    vec3 displacedUp=displace(vertexPosition+vec3(0,0.01,0));

    vec3 a=displacedRight-displaced;
    vec3 b=displacedUp-displaced;
    vec3 nrm=normalize(cross(a,b));
    normal=normalize(normalMatrix*nrm);
}