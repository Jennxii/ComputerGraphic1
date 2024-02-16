#version 450 core

layout ( location = 0 ) out vec4 fragmentColor;

in vec3 color;
in vec2 uvA;
//in vec2 uvB;

in vec3 normal;
in vec4 worldPosition;

uniform sampler2D textureA;
uniform sampler2D textureB;


// uniforms related to the material


// uniforms related to directional light: light color, and constant light direction
uniform vec3 directionalLightColor = vec3(1.0);
uniform vec3 directionalLightDirection = vec3(-1.0);

// uniforms related to ambient light: just a (typically dark) light color
uniform vec3 ambientLightColor = vec3(0.05);

//uniforms related to spot light
uniform vec3 spotlightColor = vec3(1.0);
uniform vec3 spotlightPosition = vec3(1.0);
uniform vec3 spotlightDirection = vec3(1.0);
uniform float spotlightAngle = float(1.0);



void main()
{

	vec4 texColorA = texture(textureA, uvA);
	vec4 texColorB = texture(textureB, uvA);

	vec4 texColor = texColorA * texColorB;

	vec3 N = normalize(normal);
	if (!gl_FrontFacing)
		N = -1 * N;

	// ambient light
	vec3 ambientContribution = ambientLightColor * texColor.rgb;
	
	// directional light
	// remember that the light direction L points away from the fragment
	vec3 L_directional = normalize(-directionalLightDirection);		
	vec3 directionalContribution = max(dot(N, L_directional), 0.0) * directionalLightColor * texColor.rgb;

	//spotlight
	vec3 L_spotlight = normalize(spotlightPosition - worldPosition.xyz);
    float angleCosine = dot(normalize(-L_spotlight), normalize(spotlightDirection))/ (length(normalize(-L_spotlight))*length(normalize(spotlightDirection)));
	float angle = acos(angleCosine); // Convert angle cosine to degrees
	
	float innerAngle = spotlightAngle * 0.80;
	
    vec3 spotlightContribution = vec3(0.0);
	
    if (angle <= spotlightAngle) {
			spotlightContribution = max(dot(N, L_spotlight), 0.0) * spotlightColor * texColor.rgb ;
	
		if (innerAngle > angle)
		{
			
			fragmentColor = vec4(directionalContribution + ambientContribution + spotlightContribution, 1.0);
		
		}
		else
		{
			fragmentColor = vec4(directionalContribution + ambientContribution + smoothstep(spotlightAngle , innerAngle , angle) * spotlightContribution, 1.0);
		
		}
	}
	else{
			fragmentColor = vec4(directionalContribution + ambientContribution, 1.0);
	
	}

}