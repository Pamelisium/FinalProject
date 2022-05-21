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

// Uniform variables for object
uniform vec3 objectSpecular;
uniform float shininess;

// Uniform variable for camera
uniform vec3 cameraPosition;

void main()
{
	// Using the Phong lighting equation to calculate the final fragment color
	vec3 normal = normalize(outNormal);
	vec3 lightDirection = normalize(lightPosition - outPosition);
	vec3 cameraDirection = normalize(cameraPosition - outPosition);
	vec3 reflection = reflect(-lightDirection, normal);

	float diffuseStrength = max(dot(normal, lightDirection), 0.0);
	float specularStrength = pow(max(dot(reflection, cameraDirection), 0.0), shininess);

	fragColor = vec4(lightAmbient + lightDiffuse * diffuseStrength + lightSpecular * objectSpecular * specularStrength, 1.0) * texture(tex, outUV);
}