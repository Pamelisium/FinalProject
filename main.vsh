#version 330

// Vertex position
layout(location = 0) in vec3 vertexPosition;

// Vertex color
layout(location = 1) in vec3 vertexColor;

// Vertex UV coordinate
layout(location = 2) in vec2 vertexUV;

// Vertex normal vector
layout(location = 3) in vec3 vertexNormal;

// Uniform variables
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normMatrix;

// UV coordinate (will be passed to the fragment shader)
out vec2 outUV;

// Color (will be passed to the fragment shader)
out vec3 outColor;

// Position (will be passed to the fragment shader)
out vec3 outPosition;

// Normal Vector (will be passed to the fragment shader)
out vec3 outNormal;

void main()
{
	gl_Position = proj * view * model * vec4(vertexPosition, 1.0);
	outUV = vertexUV;
	outColor = vertexColor;
	outPosition = vec3(model * vec4(vertexPosition, 1.0));
	outNormal = vec3(normMatrix * vec4(vertexNormal, 1.0));
}
