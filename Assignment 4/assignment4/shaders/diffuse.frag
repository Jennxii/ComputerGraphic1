#version 450 core

layout ( location = 0 ) out vec4 fragmentColor;

in vec3 normal;

// uniforms related to the material
uniform vec3 kD = vec3(0.5);

// uniforms related to directional light: light color, and constant light direction
uniform vec3 directionalLightColor = vec3(1.0);
uniform vec3 directionalLightDirection = vec3(-1.0);

// uniforms related to ambient light: just a (typically dark) light color
uniform vec3 ambientLightColor = vec3(0.05);

void main()
{
	vec3 N = normalize(normal);
	if (!gl_FrontFacing)
		N = -1 * N;

	// ambient light
	vec3 ambientContribution = ambientLightColor * kD;
	
	// directional light
	// remember that the light direction L points away from the fragment
	vec3 L_directional = normalize(-directionalLightDirection);		
	vec3 directionalContribution = max(dot(N, L_directional), 0.0) * directionalLightColor * kD;

	// final color = sum of all light sources in the scene
	// here: ambient + directional
	fragmentColor = vec4(directionalContribution + ambientContribution, 1.0);
}