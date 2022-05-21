// Quick note: GLAD needs to be included first before GLFW.
// Otherwise, GLAD will complain about gl.h being already included.
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// ---------------
// Function declarations
// ---------------

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath);

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource);

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height);

/// <summary>
/// Struct containing data about a vertex
/// </summary>
struct Vertex
{
	GLfloat x, y, z;	// Position
	GLubyte r, g, b;	// Color
	GLfloat u, v;		// UV Coordinates
	GLfloat nx, ny, nz; // Normal Vector
};

/// <summary>
/// Main function.
/// </summary>
/// <returns>An integer indicating whether the program ended successfully or not.
/// A value of 0 indicates the program ended succesfully, while a non-zero value indicates
/// something wrong happened during execution.</returns>
int main()
{
	// Initialize GLFW
	int glfwInitStatus = glfwInit();
	if (glfwInitStatus == GLFW_FALSE)
	{
		std::cerr << "Failed to initialize GLFW!" << std::endl;
		return 1;
	}

	// Tell GLFW that we prefer to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Tell GLFW that we prefer to use the modern OpenGL
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tell GLFW to create a window
	int windowWidth = 800;
	int windowHeight = 600;
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Lighting", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Tell GLFW to use the OpenGL context that was assigned to the window that we just created
	glfwMakeContextCurrent(window);

	// Register the callback function that handles when the framebuffer size has changed
	glfwSetFramebufferSizeCallback(window, FramebufferSizeChangedCallback);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---
	
	// Set up the data for each vertex of the triangle
	Vertex vertices[24];

	// Front Square
	vertices[0].x = -0.5f;			vertices[0].y = -0.5f;			vertices[0].z = -0.5f;
	vertices[0].r = 255;			vertices[0].g = 255;			vertices[0].b = 255;
	vertices[0].u = 0.0f;			vertices[0].v = (float)2/3;
	vertices[0].nx = 0.0f;			vertices[0].ny = 0.0f;			vertices[0].nz = -1.0f;

	vertices[1].x = -0.5f;			vertices[1].y = 0.5f;			vertices[1].z = -0.5f;
	vertices[1].r = 255;			vertices[1].g = 255;			vertices[1].b = 255;
	vertices[1].u = 0.0f;			vertices[1].v = 1.0f;
	vertices[1].nx = 0.0f;			vertices[1].ny = 0.0f;			vertices[1].nz = -1.0f;

	vertices[2].x = 0.5f;			vertices[2].y = 0.5f;			vertices[2].z = -0.5f;
	vertices[2].r = 255;			vertices[2].g = 255;			vertices[2].b = 255;
	vertices[2].u = (float)1/3;		vertices[2].v = 1.0f;
	vertices[2].nx = 0.0f;			vertices[2].ny = 0.0f;			vertices[2].nz = -1.0f;

	vertices[3].x = 0.5f;			vertices[3].y = -0.5f;			vertices[3].z = -0.5f;
	vertices[3].r = 255;			vertices[3].g = 255;			vertices[3].b = 255;
	vertices[3].u = (float)1/3;		vertices[3].v = (float)2/3;
	vertices[3].nx = 0.0f;			vertices[3].ny = 0.0f;			vertices[3].nz = -1.0f;

	// Back Square
	vertices[4].x = 0.5f;			vertices[4].y = -0.5f;			vertices[4].z = 0.5f;
	vertices[4].r = 255;			vertices[4].g = 255;			vertices[4].b = 255;
	vertices[4].u = (float)2/3;		vertices[4].v = (float)1/3;
	vertices[4].nx = 0.0f;			vertices[4].ny = 0.0f;			vertices[4].nz = 1.0f;

	vertices[5].x = 0.5f;			vertices[5].y = 0.5f;			vertices[5].z = 0.5f;
	vertices[5].r = 255;			vertices[5].g = 255;			vertices[5].b = 255;
	vertices[5].u = (float)2/3;		vertices[5].v = (float)2/3;
	vertices[5].nx = 0.0f;			vertices[5].ny = 0.0f;			vertices[5].nz = 1.0f;

	vertices[6].x = -0.5f;			vertices[6].y = 0.5f;			vertices[6].z = 0.5f;
	vertices[6].r = 255;			vertices[6].g = 255;			vertices[6].b = 255;
	vertices[6].u = 1.0f;			vertices[6].v = (float)2/3;
	vertices[6].nx = 0.0f;			vertices[6].ny = 0.0f;			vertices[6].nz = 1.0f;

	vertices[7].x = -0.5f;			vertices[7].y = -0.5f;			vertices[7].z = 0.5f;
	vertices[7].r = 255;			vertices[7].g = 255;			vertices[7].b = 255;
	vertices[7].u = 1.0f;			vertices[7].v = (float)1/3;
	vertices[7].nx = 0.0f;			vertices[7].ny = 0.0f;			vertices[7].nz = 1.0f;

	// Left Square
	vertices[8].x = -0.5f;			vertices[8].y = -0.5f;			vertices[8].z = 0.5f;
	vertices[8].r = 255;			vertices[8].g = 255;			vertices[8].b = 255;
	vertices[8].u = 0.0f;			vertices[8].v = (float)1/3;
	vertices[8].nx = -1.0f;			vertices[8].ny = 0.0f;			vertices[8].nz = 0.0f;

	vertices[9].x = -0.5f;			vertices[9].y = 0.5f;			vertices[9].z = 0.5f;
	vertices[9].r = 255;			vertices[9].g = 255;			vertices[9].b = 255;
	vertices[9].u = 0.0f;			vertices[9].v = (float)2/3;
	vertices[9].nx = -1.0f;			vertices[9].ny = 0.0f;			vertices[9].nz = 0.0f;

	vertices[10].x = -0.5f;			vertices[10].y = 0.5f;			vertices[10].z = -0.5f;
	vertices[10].r = 255;			vertices[10].g = 255;			vertices[10].b = 255;
	vertices[10].u = (float)1/3;	vertices[10].v = (float)2/3;
	vertices[10].nx = -1.0f;		vertices[10].ny = 0.0f;			vertices[10].nz = 0.0f;

	vertices[11].x = -0.5f;			vertices[11].y = -0.5f;			vertices[11].z = -0.5f;
	vertices[11].r = 255;			vertices[11].g = 255;			vertices[11].b = 255;
	vertices[11].u = (float)1/3;	vertices[11].v = (float)1/3;
	vertices[11].nx = -1.0f;		vertices[11].ny = 0.0f;			vertices[11].nz = 0.0f;

	// Right Square
	vertices[12].x = 0.5f;			vertices[12].y = -0.5f;			vertices[12].z = -0.5f;
	vertices[12].r = 255;			vertices[12].g = 255;			vertices[12].b = 255;
	vertices[12].u = (float)2/3;	vertices[12].v = (float)2/3;
	vertices[12].nx = 1.0f;			vertices[12].ny = 0.0f;			vertices[12].nz = 0.0f;

	vertices[13].x = 0.5f;			vertices[13].y = 0.5f;			vertices[13].z = -0.5f;
	vertices[13].r = 255;			vertices[13].g = 255;			vertices[13].b = 255;
	vertices[13].u = (float)2/3;	vertices[13].v = 1.0f;
	vertices[13].nx = 1.0f;			vertices[13].ny = 0.0f;			vertices[13].nz = 0.0f;

	vertices[14].x = 0.5f;			vertices[14].y = 0.5f;			vertices[14].z = 0.5f;
	vertices[14].r = 255;			vertices[14].g = 255;			vertices[14].b = 255;
	vertices[14].u = 1.0f;			vertices[14].v = 1.0f;
	vertices[14].nx = 1.0f;			vertices[14].ny = 0.0f;			vertices[14].nz = 0.0f;

	vertices[15].x = 0.5f;			vertices[15].y = -0.5f;			vertices[15].z = 0.5f;
	vertices[15].r = 255;			vertices[15].g = 255;			vertices[15].b = 255;
	vertices[15].u = 1.0f;			vertices[15].v = (float)2/3;
	vertices[15].nx = 1.0f;			vertices[15].ny = 0.0f;			vertices[15].nz = 0.0f;

	// Top Square
	vertices[16].x = -0.5f;			vertices[16].y = 0.5f;			vertices[16].z = -0.5f;
	vertices[16].r = 255;			vertices[16].g = 255;			vertices[16].b = 255;
	vertices[16].u = (float)1/3;	vertices[16].v = (float)2/3;
	vertices[16].nx = 0.0f;			vertices[16].ny = 1.0f;			vertices[16].nz = 0.0f;

	vertices[17].x = -0.5f;			vertices[17].y = 0.5f;			vertices[17].z = 0.5f;
	vertices[17].r = 255;			vertices[17].g = 255;			vertices[17].b = 255;
	vertices[17].u = (float)1/3;	vertices[17].v = 1.0f;
	vertices[17].nx = 0.0f;			vertices[17].ny = 1.0f;			vertices[17].nz = 0.0f;

	vertices[18].x = 0.5f;			vertices[18].y = 0.5f;			vertices[18].z = 0.5f;
	vertices[18].r = 255;			vertices[18].g = 255;			vertices[18].b = 255;
	vertices[18].u = (float)2/3;	vertices[18].v = 1.0f;
	vertices[18].nx = 0.0f;			vertices[18].ny = 1.0f;			vertices[18].nz = 0.0f;

	vertices[19].x = 0.5f;			vertices[19].y = 0.5f;			vertices[19].z = -0.5f;
	vertices[19].r = 255;			vertices[19].g = 255;			vertices[19].b = 255;
	vertices[19].u = (float)2/3;	vertices[19].v = (float)2/3;
	vertices[19].nx = 0.0f;			vertices[19].ny = 1.0f;			vertices[19].nz = 0.0f;

	// Bottom Square
	vertices[20].x = 0.5f;			vertices[20].y = -0.5f;			vertices[20].z = -0.5f;
	vertices[20].r = 255;			vertices[20].g = 255;			vertices[20].b = 255;
	vertices[20].u = (float)1/3;	vertices[20].v = (float)1/3;
	vertices[20].nx = 0.0f;			vertices[20].ny = -1.0f;		vertices[20].nz = 0.0f;

	vertices[21].x = 0.5f;			vertices[21].y = -0.5f;			vertices[21].z = 0.5f;
	vertices[21].r = 255;			vertices[21].g = 255;			vertices[21].b = 255;
	vertices[21].u = (float)1/3;	vertices[21].v = (float)2/3;
	vertices[21].nx = 0.0f;			vertices[21].ny = -1.0f;		vertices[21].nz = 0.0f;

	vertices[22].x = -0.5f;			vertices[22].y = -0.5f;			vertices[22].z = 0.5f;
	vertices[22].r = 255;			vertices[22].g = 255;			vertices[22].b = 255;
	vertices[22].u = (float)2/3;	vertices[22].v = (float)2/3;
	vertices[22].nx = 0.0f;			vertices[22].ny = -1.0f;		vertices[22].nz = 0.0f;

	vertices[23].x = -0.5f;			vertices[23].y = -0.5f;			vertices[23].z = -0.5f;
	vertices[23].r = 255;			vertices[23].g = 255;			vertices[23].b = 255;
	vertices[23].u = (float)2/3;	vertices[23].v = (float)1/3;
	vertices[23].nx = 0.0f;			vertices[23].ny = -1.0f;		vertices[23].nz = 0.0f;

	// Create a vertex buffer object (VBO), and upload our vertices data to the VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create a vertex array object that contains data on how to map vertex attributes
	// (e.g., position, color) to vertex shader properties.
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	// Vertex attribute 0 - Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

	// Vertex attribute 1 - Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(offsetof(Vertex, r)));

	// Vertex attribute 2 - UV Coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, u)));
	
	// Vertex attribute 3 - Normal Vector
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, nx)));

	glBindVertexArray(0);

	// Create a shader program
	GLuint program = CreateShaderProgram("main.vsh", "main.fsh");

	// Tell OpenGL the dimensions of the region where stuff will be drawn.
	// For now, tell OpenGL to use the whole screen
	glViewport(0, 0, windowWidth, windowHeight);

	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint tex;
	glGenTextures(1, &tex);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);

	// 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
	int imageWidth, imageHeight, numChannels;

	// Read the image data and store it in an unsigned char array
	unsigned char* imageData = stbi_load("cube.png", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, tex);

		// Set the filtering methods for magnification and minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Set the wrapping method for the s-axis (x-axis) and t-axis (y-axis)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload the image data to GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		// If we set minification to use mipmaps, we can tell OpenGL to generate the mipmaps for us
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Once we have copied the data over to the GPU, we can delete
		// the data on the CPU side, since we won't be using it anymore
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Clear the colors in our off-screen framebuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the shader program that we created
		glUseProgram(program);

		// Use the vertex array object that we created
		glBindVertexArray(vao);

		// Bind our texture to texture unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		// Make our sampler in the fragment shader use texture unit 0
		GLint texUniformLocation = glGetUniformLocation(program, "tex");
		glUniform1i(texUniformLocation, 0);

		// Uniform variables for point light
		GLint lightPositionUniformLocation = glGetUniformLocation(program, "lightPosition");
		glUniform3f(lightPositionUniformLocation, 0.0f, 0.0f, 3.0f);

		GLint lightAmbientUniformLocation = glGetUniformLocation(program, "lightAmbient");
		glUniform3f(lightAmbientUniformLocation, 0.2f, 0.2f, 0.2f);

		GLint lightDiffuseUniformLocation = glGetUniformLocation(program, "lightDiffuse");
		glUniform3f(lightDiffuseUniformLocation, 1.0f, 1.0f, 1.0f);

		GLint lightSpecularUniformLocation = glGetUniformLocation(program, "lightSpecular");
		glUniform3f(lightSpecularUniformLocation, 0.5f, 0.5f, 0.5f);

		// Uniform variables for object
		GLint objectSpecularUniformLocation = glGetUniformLocation(program, "objectSpecular");
		glUniform3f(objectSpecularUniformLocation, 0.5f, 0.5f, 0.5f);

		GLint shininessUniformLocation = glGetUniformLocation(program, "shininess");
		glUniform1f(shininessUniformLocation, 8.0f);

		// DIE 1

		// Projection Matrix
		glm::mat4 proj = glm::perspective(glm::radians(90.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		// View Matrix
		glm::vec3 cameraPosition = glm::vec3 (0.5f, 0.0f, 1.25f);
		glm::mat4 view = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Model Matrix
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
		model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		// Normal Matrix
		glm::mat4 normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		GLint projUniformLocation = glGetUniformLocation(program, "proj");
    	glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(proj));

		GLint viewUniformLocation = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		GLint cameraPositionUniformLocation = glGetUniformLocation(program, "cameraPosition");
		glUniform3fv(cameraPositionUniformLocation, 3, glm::value_ptr(cameraPosition));

		GLint modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		GLint normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

		// DIE 2

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

		// DIE 3

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -1.0f, -0.2f));
		model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

		// DIE 4

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f+(sin((float)glfwGetTime())*0.8f), 0.0f+(cos((float)glfwGetTime())*0.8f)));
		model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

		// DIE 5

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f+(sin((float)glfwGetTime())*0.2f), -1.0f, -0.2f+(cos((float)glfwGetTime())*0.2f)));
		model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

		// "Unuse" the vertex array object
		glBindVertexArray(0);

		// Tell GLFW to swap the screen buffer with the offscreen buffer
		glfwSwapBuffers(window);

		// Tell GLFW to process window events (e.g., input events, window closed events, etc.)
		glfwPollEvents();
	}

	// --- Cleanup ---

	// Make sure to delete the shader program
	glDeleteProgram(program);

	// Delete the VBO that contains our vertices
	glDeleteBuffers(1, &vbo);

	// Delete the vertex array object
	glDeleteVertexArrays(1, &vao);

	// Remember to tell GLFW to clean itself up before exiting the application
	glfwTerminate();

	return 0;
}

/// <summary>
/// Creates a shader program based on the provided file paths for the vertex and fragment shaders.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path</param>
/// <returns>OpenGL handle to the created shader program</returns>
GLuint CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	GLuint vertexShader = CreateShaderFromFile(GL_VERTEX_SHADER, vertexShaderFilePath);
	GLuint fragmentShader = CreateShaderFromFile(GL_FRAGMENT_SHADER, fragmentShaderFilePath);

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	// Check shader program link status
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) {
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetProgramInfoLog(program, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "program link error: " << infoLog << std::endl;
	}

	return program;
}

/// <summary>
/// Creates a shader based on the provided shader type and the path to the file containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderFilePath">Path to the file containing the shader source</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromFile(const GLuint& shaderType, const std::string& shaderFilePath)
{
	std::ifstream shaderFile(shaderFilePath);
	if (shaderFile.fail())
	{
		std::cerr << "Unable to open shader file: " << shaderFilePath << std::endl;
		return 0;
	}

	std::string shaderSource;
	std::string temp;
	while (std::getline(shaderFile, temp))
	{
		shaderSource += temp + "\n";
	}
	shaderFile.close();

	return CreateShaderFromSource(shaderType, shaderSource);
}

/// <summary>
/// Creates a shader based on the provided shader type and the string containing the shader source.
/// </summary>
/// <param name="shaderType">Shader type</param>
/// <param name="shaderSource">Shader source string</param>
/// <returns>OpenGL handle to the created shader</returns>
GLuint CreateShaderFromSource(const GLuint& shaderType, const std::string& shaderSource)
{
	GLuint shader = glCreateShader(shaderType);

	const char* shaderSourceCStr = shaderSource.c_str();
	GLint shaderSourceLen = static_cast<GLint>(shaderSource.length());
	glShaderSource(shader, 1, &shaderSourceCStr, &shaderSourceLen);
	glCompileShader(shader);

	// Check compilation status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
	{
		char infoLog[512];
		GLsizei infoLogLen = sizeof(infoLog);
		glGetShaderInfoLog(shader, infoLogLen, &infoLogLen, infoLog);
		std::cerr << "shader compilation error: " << infoLog << std::endl;
	}

	return shader;
}

/// <summary>
/// Function for handling the event when the size of the framebuffer changed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="width">New width</param>
/// <param name="height">New height</param>
void FramebufferSizeChangedCallback(GLFWwindow* window, int width, int height)
{
	// Whenever the size of the framebuffer changed (due to window resizing, etc.),
	// update the dimensions of the region to the new size
	glViewport(0, 0, width, height);
}
