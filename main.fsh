#version 330

// UV-coordinate of the fragment (interpolated by the rasterization stage)
in vec2 outUV;

// Color of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outColor;

// Position of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outPosition;

// Normal vector of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outNormal;

// Final color of the fragment that will be rendered on the screen
out vec4 fragColor;

// Texture unit of the texture
uniform sampler2D tex;

// Uniform variables for point light
uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

// Uniform variables for spot light
uniform vec3 spotlightPosition[4];
uniform vec3 spotlightAmbient;
uniform vec3 spotlightDiffuse;
uniform vec3 spotlightSpecular;
uniform vec3 spotlightTarget;
uniform float spotlightCutoff;

// Uniform variables for object
uniform vec3 objectSpecular;
uniform float shininess;

// Uniform variable for camera
uniform vec3 cameraPosition;

void main()
{
	vec3 normal = normalize(outNormal);
	vec3 cameraDirection = normalize(cameraPosition - outPosition);
	vec3 lightSum = vec3(0.0);

	// Using the Phong lighting equation to calculate the final fragment color considering point light
	vec3 lightDirection = normalize(lightPosition - outPosition);
	vec3 reflection = reflect(-lightDirection, normal);

	float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	float specularStrength = pow(max(dot(reflection, cameraDirection), 0.0), shininess);

	lightSum += lightAmbient + lightDiffuse * diffuseStrength + lightSpecular * objectSpecular * specularStrength;

	// Using the Phong lighting equation to calculate the final fragment color considering spot light
	for (int i = 0; i < 4; i++){
		vec3 spotlightDirection = normalize(spotlightPosition[i] - outPosition);
		vec3 spotlightReflection = reflect(-spotlightDirection, normal);

		float spotlightDiffuseStrength = max(dot(normal, spotlightDirection), 0.0);
		float spotlightSpecularStrength = pow(max(dot(spotlightReflection, cameraDirection), 0.0), shininess);
		float spotFactor = dot(spotlightDirection, normalize(-spotlightTarget));
	
		if (spotFactor > spotlightCutoff){
			lightSum += spotlightAmbient + spotlightDiffuse * spotlightDiffuseStrength + spotlightSpecular * objectSpecular * spotlightSpecularStrength;
		}
		else{
			fragColor = vec4(spotlightAmbient, 1.0) * texture(tex, outUV);
		}
	}

	// Combining point and spot lights to produce final fragment color
	fragColor = vec4(lightSum, 1.0) * texture(tex, outUV);
}