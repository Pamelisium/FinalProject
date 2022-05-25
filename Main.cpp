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
/// Function for handling the event when a key is pressed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="key">Keyboard key</param>
/// <param name="scancode">Platform-specific scancode</param>
/// <param name="action">Action done on key</param>
/// <param name="mods">Modifier bits</param>
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/// <summary>
/// Function for handling the event when the mouse moves.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="xpos">Mouse current x-position</param>
/// <param name="ypos">Mouse current y-position</param>
void MouseCallback(GLFWwindow* window, double xpos, double ypos);

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
/// Camera variables
/// </summary>
glm::vec3 cameraPosition = glm::vec3(-23.0f, -15.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

/// <summary>
/// Angle that depicts the amount of looking up or down 
/// </summary>
float pitch;

/// <summary>
/// Angle that depicts the amount of looking left or right
/// </summary>
float yaw;

/// <summary>
/// Initial mouse position at the center of the screen
/// </summary>
float lastX, lastY;

/// <summary>
/// Indicates if mouse input is received for the first time
/// </summary>
bool firstMouse = true;

/// <summary>
/// Indicates the visibility of mouse cursor
/// </summary>
bool visibleCursor = false;

/// <summary>
/// Indicates the previous camera x-position before change
/// </summary>
float previousX = cameraPosition.x;

/// <summary>
/// Indicates the previous camera z-position before change
/// </summary>
float previousZ = cameraPosition.z;

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
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "[Mendoza & Serrano] GDEV 30 Final Project", nullptr, nullptr);
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

	// Register the callback function that handles when a key is pressed
	glfwSetKeyCallback(window, KeyCallback);

	// Register the callback function that handles when the mouse moves
	glfwSetCursorPosCallback(window, MouseCallback);

	// Tell OpenGL to hide and capture the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Tell GLAD to load the OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initialize GLAD!" << std::endl;
		return 1;
	}

	// --- Vertex specification ---
	
	// Set up the data for each vertex of the triangle
	Vertex vertices[84+56334+2904+13984+11988];

	// --- Room ---

	// Front Wall
	vertices[0].x = -0.5f;			vertices[0].y = -0.5f;			vertices[0].z = -0.5f;
	vertices[0].r = 255;			vertices[0].g = 255;			vertices[0].b = 255;
	vertices[0].u = 0.0f;			vertices[0].v = 0.0f;
	vertices[0].nx = 0.0f;			vertices[0].ny = 0.0f;			vertices[0].nz = 1.0f;

	vertices[1].x = -0.5f;			vertices[1].y = 0.5f;			vertices[1].z = -0.5f;
	vertices[1].r = 255;			vertices[1].g = 255;			vertices[1].b = 255;
	vertices[1].u = 0.0f;			vertices[1].v = 1.0f;
	vertices[1].nx = 0.0f;			vertices[1].ny = 0.0f;			vertices[1].nz = 1.0f;

	vertices[2].x = 0.5f;			vertices[2].y = 0.5f;			vertices[2].z = -0.5f;
	vertices[2].r = 255;			vertices[2].g = 255;			vertices[2].b = 255;
	vertices[2].u = 1.0f;			vertices[2].v = 1.0f;
	vertices[2].nx = 0.0f;			vertices[2].ny = 0.0f;			vertices[2].nz = 1.0f;

	vertices[3].x = 0.5f;			vertices[3].y = -0.5f;			vertices[3].z = -0.5f;
	vertices[3].r = 255;			vertices[3].g = 255;			vertices[3].b = 255;
	vertices[3].u = 1.0f;			vertices[3].v = 0.0f;
	vertices[3].nx = 0.0f;			vertices[3].ny = 0.0f;			vertices[3].nz = 1.0f;

	// Back Wall
	vertices[4].x = 0.5f;			vertices[4].y = -0.5f;			vertices[4].z = 0.5f;
	vertices[4].r = 255;			vertices[4].g = 255;			vertices[4].b = 255;
	vertices[4].u = 0.0f;			vertices[4].v = 0.0f;
	vertices[4].nx = 0.0f;			vertices[4].ny = 0.0f;			vertices[4].nz = -1.0f;

	vertices[5].x = 0.5f;			vertices[5].y = 0.5f;			vertices[5].z = 0.5f;
	vertices[5].r = 255;			vertices[5].g = 255;			vertices[5].b = 255;
	vertices[5].u = 0.0f;			vertices[5].v = 1.0f;
	vertices[5].nx = 0.0f;			vertices[5].ny = 0.0f;			vertices[5].nz = -1.0f;

	vertices[6].x = -0.5f;			vertices[6].y = 0.5f;			vertices[6].z = 0.5f;
	vertices[6].r = 255;			vertices[6].g = 255;			vertices[6].b = 255;
	vertices[6].u = 1.0f;			vertices[6].v = 1.0f;
	vertices[6].nx = 0.0f;			vertices[6].ny = 0.0f;			vertices[6].nz = -1.0f;

	vertices[7].x = -0.5f;			vertices[7].y = -0.5f;			vertices[7].z = 0.5f;
	vertices[7].r = 255;			vertices[7].g = 255;			vertices[7].b = 255;
	vertices[7].u = 1.0f;			vertices[7].v = 0.0f;
	vertices[7].nx = 0.0f;			vertices[7].ny = 0.0f;			vertices[7].nz = -1.0f;

	// Left Wall
	vertices[8].x = -0.5f;			vertices[8].y = -0.5f;			vertices[8].z = 0.5f;
	vertices[8].r = 255;			vertices[8].g = 255;			vertices[8].b = 255;
	vertices[8].u = 0.0f;			vertices[8].v = 0.0f;
	vertices[8].nx = 1.0f;			vertices[8].ny = 0.0f;			vertices[8].nz = 0.0f;

	vertices[9].x = -0.5f;			vertices[9].y = 0.5f;			vertices[9].z = 0.5f;
	vertices[9].r = 255;			vertices[9].g = 255;			vertices[9].b = 255;
	vertices[9].u = 0.0f;			vertices[9].v = 1.0f;
	vertices[9].nx = 1.0f;			vertices[9].ny = 0.0f;			vertices[9].nz = 0.0f;

	vertices[10].x = -0.5f;			vertices[10].y = 0.5f;			vertices[10].z = -0.5f;
	vertices[10].r = 255;			vertices[10].g = 255;			vertices[10].b = 255;
	vertices[10].u = 1.0f;			vertices[10].v = 1.0f;
	vertices[10].nx = 1.0f;			vertices[10].ny = 0.0f;			vertices[10].nz = 0.0f;

	vertices[11].x = -0.5f;			vertices[11].y = -0.5f;			vertices[11].z = -0.5f;
	vertices[11].r = 255;			vertices[11].g = 255;			vertices[11].b = 255;
	vertices[11].u = 1.0f;			vertices[11].v = 0.0f;
	vertices[11].nx = 1.0f;			vertices[11].ny = 0.0f;			vertices[11].nz = 0.0f;

	// Right Wall
	vertices[12].x = 0.5f;			vertices[12].y = -0.5f;			vertices[12].z = -0.5f;
	vertices[12].r = 255;			vertices[12].g = 255;			vertices[12].b = 255;
	vertices[12].u = 1.0f;			vertices[12].v = 0.0f;
	vertices[12].nx = -1.0f;		vertices[12].ny = 0.0f;			vertices[12].nz = 0.0f;

	vertices[13].x = 0.5f;			vertices[13].y = 0.5f;			vertices[13].z = -0.5f;
	vertices[13].r = 255;			vertices[13].g = 255;			vertices[13].b = 255;
	vertices[13].u = 1.0f;			vertices[13].v = 1.0f;
	vertices[13].nx = -1.0f;		vertices[13].ny = 0.0f;			vertices[13].nz = 0.0f;

	vertices[14].x = 0.5f;			vertices[14].y = 0.5f;			vertices[14].z = 0.5f;
	vertices[14].r = 255;			vertices[14].g = 255;			vertices[14].b = 255;
	vertices[14].u = 0.0f;			vertices[14].v = 1.0f;
	vertices[14].nx = -1.0f;		vertices[14].ny = 0.0f;			vertices[14].nz = 0.0f;

	vertices[15].x = 0.5f;			vertices[15].y = -0.5f;			vertices[15].z = 0.5f;
	vertices[15].r = 255;			vertices[15].g = 255;			vertices[15].b = 255;
	vertices[15].u = 0.0f;			vertices[15].v = 0.0f;
	vertices[15].nx = -1.0f;		vertices[15].ny = 0.0f;			vertices[15].nz = 0.0f;

	// Ceiling
	vertices[16].x = -0.5f;			vertices[16].y = 0.5f;			vertices[16].z = -0.5f;
	vertices[16].r = 255;			vertices[16].g = 255;			vertices[16].b = 255;
	vertices[16].u = 0.0f;			vertices[16].v = 0.0f;
	vertices[16].nx = 0.0f;			vertices[16].ny = -1.0f;		vertices[16].nz = 0.0f;

	vertices[17].x = -0.5f;			vertices[17].y = 0.5f;			vertices[17].z = 0.5f;
	vertices[17].r = 255;			vertices[17].g = 255;			vertices[17].b = 255;
	vertices[17].u = 0.0f;			vertices[17].v = 1.0f;
	vertices[17].nx = 0.0f;			vertices[17].ny = -1.0f;		vertices[17].nz = 0.0f;

	vertices[18].x = 0.5f;			vertices[18].y = 0.5f;			vertices[18].z = 0.5f;
	vertices[18].r = 255;			vertices[18].g = 255;			vertices[18].b = 255;
	vertices[18].u = 1.0f;			vertices[18].v = 1.0f;
	vertices[18].nx = 0.0f;			vertices[18].ny = -1.0f;		vertices[18].nz = 0.0f;

	vertices[19].x = 0.5f;			vertices[19].y = 0.5f;			vertices[19].z = -0.5f;
	vertices[19].r = 255;			vertices[19].g = 255;			vertices[19].b = 255;
	vertices[19].u = 1.0f;			vertices[19].v = 0.0f;
	vertices[19].nx = 0.0f;			vertices[19].ny = -1.0f;		vertices[19].nz = 0.0f;

	// Floor
	vertices[20].x = 0.5f;			vertices[20].y = -0.5f;			vertices[20].z = -0.5f;
	vertices[20].r = 255;			vertices[20].g = 255;			vertices[20].b = 255;
	vertices[20].u = 0.0f;			vertices[20].v = 0.0f;
	vertices[20].nx = 0.0f;			vertices[20].ny = 1.0f;			vertices[20].nz = 0.0f;

	vertices[21].x = 0.5f;			vertices[21].y = -0.5f;			vertices[21].z = 0.5f;
	vertices[21].r = 255;			vertices[21].g = 255;			vertices[21].b = 255;
	vertices[21].u = 0.0f;			vertices[21].v = 1.0f;
	vertices[21].nx = 0.0f;			vertices[21].ny = 1.0f;			vertices[21].nz = 0.0f;

	vertices[22].x = -0.5f;			vertices[22].y = -0.5f;			vertices[22].z = 0.5f;
	vertices[22].r = 255;			vertices[22].g = 255;			vertices[22].b = 255;
	vertices[22].u = 1.0f;			vertices[22].v = 1.0f;
	vertices[22].nx = 0.0f;			vertices[22].ny = 1.0f;			vertices[22].nz = 0.0f;

	vertices[23].x = -0.5f;			vertices[23].y = -0.5f;			vertices[23].z = -0.5f;
	vertices[23].r = 255;			vertices[23].g = 255;			vertices[23].b = 255;
	vertices[23].u = 1.0f;			vertices[23].v = 0.0f;
	vertices[23].nx = 0.0f;			vertices[23].ny = 1.0f;			vertices[23].nz = 0.0f;

	// --- Platform ---

	// Front Rectangle
	vertices[24].x = -0.5f;			vertices[24].y = -0.8f;			vertices[24].z = -0.5f;
	vertices[24].r = 255;			vertices[24].g = 255;			vertices[24].b = 255;
	vertices[24].u = 0.0f;			vertices[24].v = 0.0f;
	vertices[24].nx = 0.0f;			vertices[24].ny = 0.0f;			vertices[24].nz = -1.0f;

	vertices[25].x = -0.5f;			vertices[25].y = 0.8f;			vertices[25].z = -0.5f;
	vertices[25].r = 255;			vertices[25].g = 255;			vertices[25].b = 255;
	vertices[25].u = 0.0f;			vertices[25].v = 1.0f;
	vertices[25].nx = 0.0f;			vertices[25].ny = 0.0f;			vertices[25].nz = -1.0f;

	vertices[26].x = 0.5f;			vertices[26].y = 0.8f;			vertices[26].z = -0.5f;
	vertices[26].r = 255;			vertices[26].g = 255;			vertices[26].b = 255;
	vertices[26].u = 1.0f;			vertices[26].v = 1.0f;
	vertices[26].nx = 0.0f;			vertices[26].ny = 0.0f;			vertices[26].nz = -1.0f;

	vertices[27].x = 0.5f;			vertices[27].y = -0.8f;			vertices[27].z = -0.5f;
	vertices[27].r = 255;			vertices[27].g = 255;			vertices[27].b = 255;
	vertices[27].u = 1.0f;			vertices[27].v = 0.0f;
	vertices[27].nx = 0.0f;			vertices[27].ny = 0.0f;			vertices[27].nz = -1.0f;

	// Back Rectangle
	vertices[28].x = 0.5f;			vertices[28].y = -0.8f;			vertices[28].z = 0.5f;
	vertices[28].r = 255;			vertices[28].g = 255;			vertices[28].b = 255;
	vertices[28].u = 0.0f;			vertices[28].v = 0.0f;
	vertices[28].nx = 0.0f;			vertices[28].ny = 0.0f;			vertices[28].nz = 1.0f;

	vertices[29].x = 0.5f;			vertices[29].y = 0.8f;			vertices[29].z = 0.5f;
	vertices[29].r = 255;			vertices[29].g = 255;			vertices[29].b = 255;
	vertices[29].u = 0.0f;			vertices[29].v = 1.0f;
	vertices[29].nx = 0.0f;			vertices[29].ny = 0.0f;			vertices[29].nz = 1.0f;

	vertices[30].x = -0.5f;			vertices[30].y = 0.8f;			vertices[30].z = 0.5f;
	vertices[30].r = 255;			vertices[30].g = 255;			vertices[30].b = 255;
	vertices[30].u = 1.0f;			vertices[30].v = 1.0f;
	vertices[30].nx = 0.0f;			vertices[30].ny = 0.0f;			vertices[30].nz = 1.0f;

	vertices[31].x = -0.5f;			vertices[31].y = -0.8f;			vertices[31].z = 0.5f;
	vertices[31].r = 255;			vertices[31].g = 255;			vertices[31].b = 255;
	vertices[31].u = 1.0f;			vertices[31].v = 0.0f;
	vertices[31].nx = 0.0f;			vertices[31].ny = 0.0f;			vertices[31].nz = 1.0f;

	// Left Rectangle
	vertices[32].x = -0.5f;			vertices[32].y = -0.8f;			vertices[32].z = 0.5f;
	vertices[32].r = 255;			vertices[32].g = 255;			vertices[32].b = 255;
	vertices[32].u = 0.0f;			vertices[32].v = 0.0f;
	vertices[32].nx = -1.0f;		vertices[32].ny = 0.0f;			vertices[32].nz = 0.0f;

	vertices[33].x = -0.5f;			vertices[33].y = 0.8f;			vertices[33].z = 0.5f;
	vertices[33].r = 255;			vertices[33].g = 255;			vertices[33].b = 255;
	vertices[33].u = 0.0f;			vertices[33].v = 1.0f;
	vertices[33].nx = -1.0f;		vertices[33].ny = 0.0f;			vertices[33].nz = 0.0f;

	vertices[34].x = -0.5f;			vertices[34].y = 0.8f;			vertices[34].z = -0.5f;
	vertices[34].r = 255;			vertices[34].g = 255;			vertices[34].b = 255;
	vertices[34].u = 1.0f;			vertices[34].v = 1.0f;
	vertices[34].nx = -1.0f;		vertices[34].ny = 0.0f;			vertices[34].nz = 0.0f;

	vertices[35].x = -0.5f;			vertices[35].y = -0.8f;			vertices[35].z = -0.5f;
	vertices[35].r = 255;			vertices[35].g = 255;			vertices[35].b = 255;
	vertices[35].u = 1.0f;			vertices[35].v = 0.0f;
	vertices[35].nx = -1.0f;		vertices[35].ny = 0.0f;			vertices[35].nz = 0.0f;

	// Right Rectangle
	vertices[36].x = 0.5f;			vertices[36].y = -0.8f;			vertices[36].z = -0.5f;
	vertices[36].r = 255;			vertices[36].g = 255;			vertices[36].b = 255;
	vertices[36].u = 0.0f;			vertices[36].v = 0.0f;
	vertices[36].nx = 1.0f;			vertices[36].ny = 0.0f;			vertices[36].nz = 0.0f;

	vertices[37].x = 0.5f;			vertices[37].y = 0.8f;			vertices[37].z = -0.5f;
	vertices[37].r = 255;			vertices[37].g = 255;			vertices[37].b = 255;
	vertices[37].u = 0.0f;			vertices[37].v = 1.0f;
	vertices[37].nx = 1.0f;			vertices[37].ny = 0.0f;			vertices[37].nz = 0.0f;

	vertices[38].x = 0.5f;			vertices[38].y = 0.8f;			vertices[38].z = 0.5f;
	vertices[38].r = 255;			vertices[38].g = 255;			vertices[38].b = 255;
	vertices[38].u = 1.0f;			vertices[38].v = 1.0f;
	vertices[38].nx = 1.0f;			vertices[38].ny = 0.0f;			vertices[38].nz = 0.0f;

	vertices[39].x = 0.5f;			vertices[39].y = -0.8f;			vertices[39].z = 0.5f;
	vertices[39].r = 255;			vertices[39].g = 255;			vertices[39].b = 255;
	vertices[39].u = 1.0f;			vertices[39].v = 0.0f;
	vertices[39].nx = 1.0f;			vertices[39].ny = 0.0f;			vertices[39].nz = 0.0f;

	// Top Square
	vertices[40].x = -0.5f;			vertices[40].y = 0.8f;			vertices[40].z = -0.5f;
	vertices[40].r = 255;			vertices[40].g = 255;			vertices[40].b = 255;
	vertices[40].u = 0.0f;			vertices[40].v = 0.0f;
	vertices[40].nx = 0.0f;			vertices[40].ny = 1.0f;			vertices[40].nz = 0.0f;

	vertices[41].x = -0.5f;			vertices[41].y = 0.8f;			vertices[41].z = 0.5f;
	vertices[41].r = 255;			vertices[41].g = 255;			vertices[41].b = 255;
	vertices[41].u = 0.0f;			vertices[41].v = 1.0f;
	vertices[41].nx = 0.0f;			vertices[41].ny = 1.0f;			vertices[41].nz = 0.0f;

	vertices[42].x = 0.5f;			vertices[42].y = 0.8f;			vertices[42].z = 0.5f;
	vertices[42].r = 255;			vertices[42].g = 255;			vertices[42].b = 255;
	vertices[42].u = 1.0f;			vertices[42].v = 1.0f;
	vertices[42].nx = 0.0f;			vertices[42].ny = 1.0f;			vertices[42].nz = 0.0f;

	vertices[43].x = 0.5f;			vertices[43].y = 0.8f;			vertices[43].z = -0.5f;
	vertices[43].r = 255;			vertices[43].g = 255;			vertices[43].b = 255;
	vertices[43].u = 1.0f;			vertices[43].v = 0.0f;
	vertices[43].nx = 0.0f;			vertices[43].ny = 1.0f;			vertices[43].nz = 0.0f;

	// --- Square Painting ---

	// Front Square
	vertices[44].x = -0.5f;			vertices[44].y = -0.5f;			vertices[44].z = -0.5f;
	vertices[44].r = 255;			vertices[44].g = 255;			vertices[44].b = 255;
	vertices[44].u = 0.0f;			vertices[44].v = 0.0f;
	vertices[44].nx = 0.0f;			vertices[44].ny = 0.0f;			vertices[44].nz = -1.0f;

	vertices[45].x = -0.5f;			vertices[45].y = 0.5f;			vertices[45].z = -0.5f;
	vertices[45].r = 255;			vertices[45].g = 255;			vertices[45].b = 255;
	vertices[45].u = 0.0f;			vertices[45].v = 1.0f;
	vertices[45].nx = 0.0f;			vertices[45].ny = 0.0f;			vertices[45].nz = -1.0f;

	vertices[46].x = 0.5f;			vertices[46].y = 0.5f;			vertices[46].z = -0.5f;
	vertices[46].r = 255;			vertices[46].g = 255;			vertices[46].b = 255;
	vertices[46].u = 1.0f;			vertices[46].v = 1.0f;
	vertices[46].nx = 0.0f;			vertices[46].ny = 0.0f;			vertices[46].nz = -1.0f;

	vertices[47].x = 0.5f;			vertices[47].y = -0.5f;			vertices[47].z = -0.5f;
	vertices[47].r = 255;			vertices[47].g = 255;			vertices[47].b = 255;
	vertices[47].u = 1.0f;			vertices[47].v = 0.0f;
	vertices[47].nx = 0.0f;			vertices[47].ny = 0.0f;			vertices[47].nz = -1.0f;

	// Right Rectangle
	vertices[48].x = -0.5f;			vertices[48].y = -0.5f;			vertices[48].z = 0.0f;
	vertices[48].r = 255;			vertices[48].g = 255;			vertices[48].b = 255;
	vertices[48].u = 0.0f;			vertices[48].v = 0.0f;
	vertices[48].nx = -1.0f;		vertices[48].ny = 0.0f;			vertices[48].nz = 0.0f;

	vertices[49].x = -0.5f;			vertices[49].y = 0.5f;			vertices[49].z = 0.0f;
	vertices[49].r = 255;			vertices[49].g = 255;			vertices[49].b = 255;
	vertices[49].u = 0.0f;			vertices[49].v = 1.0f;
	vertices[49].nx = -1.0f;		vertices[49].ny = 0.0f;			vertices[49].nz = 0.0f;

	vertices[50].x = -0.5f;			vertices[50].y = 0.5f;			vertices[50].z = -0.5f;
	vertices[50].r = 255;			vertices[50].g = 255;			vertices[50].b = 255;
	vertices[50].u = 1.0f;			vertices[50].v = 1.0f;
	vertices[50].nx = -1.0f;		vertices[50].ny = 0.0f;			vertices[50].nz = 0.0f;

	vertices[51].x = -0.5f;			vertices[51].y = -0.5f;			vertices[51].z = -0.5f;
	vertices[51].r = 255;			vertices[51].g = 255;			vertices[51].b = 255;
	vertices[51].u = 1.0f;			vertices[51].v = 0.0f;
	vertices[51].nx = -1.0f;		vertices[51].ny = 0.0f;			vertices[51].nz = 0.0f;

	// Left Rectangle
	vertices[52].x = 0.5f;			vertices[52].y = -0.5f;			vertices[52].z = -0.5f;
	vertices[52].r = 255;			vertices[52].g = 255;			vertices[52].b = 255;
	vertices[52].u = 0.0f;			vertices[52].v = 0.0f;
	vertices[52].nx = 1.0f;			vertices[52].ny = 0.0f;			vertices[52].nz = 0.0f;

	vertices[53].x = 0.5f;			vertices[53].y = 0.5f;			vertices[53].z = -0.5f;
	vertices[53].r = 255;			vertices[53].g = 255;			vertices[53].b = 255;
	vertices[53].u = 0.0f;			vertices[53].v = 1.0f;
	vertices[53].nx = 1.0f;			vertices[53].ny = 0.0f;			vertices[53].nz = 0.0f;

	vertices[54].x = 0.5f;			vertices[54].y = 0.5f;			vertices[54].z = 0.0f;
	vertices[54].r = 255;			vertices[54].g = 255;			vertices[54].b = 255;
	vertices[54].u = 1.0f;			vertices[54].v = 1.0f;
	vertices[54].nx = 1.0f;			vertices[54].ny = 0.0f;			vertices[54].nz = 0.0f;

	vertices[55].x = 0.5f;			vertices[55].y = -0.5f;			vertices[55].z = 0.0f;
	vertices[55].r = 255;			vertices[55].g = 255;			vertices[55].b = 255;
	vertices[55].u = 1.0f;			vertices[55].v = 0.0f;
	vertices[55].nx = 1.0f;			vertices[55].ny = 0.0f;			vertices[55].nz = 0.0f;

	// Top Rectangle
	vertices[56].x = -0.5f;			vertices[56].y = 0.5f;			vertices[56].z = -0.5f;
	vertices[56].r = 255;			vertices[56].g = 255;			vertices[56].b = 255;
	vertices[56].u = 0.0f;			vertices[56].v = 0.0f;
	vertices[56].nx = 0.0f;			vertices[56].ny = 1.0f;			vertices[56].nz = 0.0f;

	vertices[57].x = -0.5f;			vertices[57].y = 0.5f;			vertices[57].z = 0.0f;
	vertices[57].r = 255;			vertices[57].g = 255;			vertices[57].b = 255;
	vertices[57].u = 0.0f;			vertices[57].v = 1.0f;
	vertices[57].nx = 0.0f;			vertices[57].ny = 1.0f;			vertices[57].nz = 0.0f;

	vertices[58].x = 0.5f;			vertices[58].y = 0.5f;			vertices[58].z = 0.0f;
	vertices[58].r = 255;			vertices[58].g = 255;			vertices[58].b = 255;
	vertices[58].u = 1.0f;			vertices[58].v = 0.0f;
	vertices[58].nx = 0.0f;			vertices[58].ny = 1.0f;			vertices[58].nz = 0.0f;

	vertices[59].x = 0.5f;			vertices[59].y = 0.5f;			vertices[59].z = -0.5f;
	vertices[59].r = 255;			vertices[59].g = 255;			vertices[59].b = 255;
	vertices[59].u = 1.0f;			vertices[59].v = 1.0f;
	vertices[59].nx = 0.0f;			vertices[59].ny = 1.0f;			vertices[59].nz = 0.0f;

	// Bottom Rectangle
	vertices[60].x = 0.5f;			vertices[60].y = -0.5f;			vertices[60].z = -0.5f;
	vertices[60].r = 255;			vertices[60].g = 255;			vertices[60].b = 255;
	vertices[60].u = 0.0f;			vertices[60].v = 0.0f;
	vertices[60].nx = 0.0f;			vertices[60].ny = -1.0f;		vertices[60].nz = 0.0f;

	vertices[61].x = 0.5f;			vertices[61].y = -0.5f;			vertices[61].z = 0.0f;
	vertices[61].r = 255;			vertices[61].g = 255;			vertices[61].b = 255;
	vertices[61].u = 0.0f;			vertices[61].v = 1.0f;
	vertices[61].nx = 0.0f;			vertices[61].ny = -1.0f;		vertices[61].nz = 0.0f;

	vertices[62].x = -0.5f;			vertices[62].y = -0.5f;			vertices[62].z = 0.0f;
	vertices[62].r = 255;			vertices[62].g = 255;			vertices[62].b = 255;
	vertices[62].u = 1.0f;			vertices[62].v = 1.0f;
	vertices[62].nx = 0.0f;			vertices[62].ny = -1.0f;		vertices[62].nz = 0.0f;

	vertices[63].x = -0.5f;			vertices[63].y = -0.5f;			vertices[63].z = -0.5f;
	vertices[63].r = 255;			vertices[63].g = 255;			vertices[63].b = 255;
	vertices[63].u = 1.0f;			vertices[63].v = 0.0f;
	vertices[63].nx = 0.0f;			vertices[63].ny = -1.0f;		vertices[63].nz = 0.0f;

	// --- Rectangular Painting ---

	// Front Rectangle
	vertices[64].x = -0.75f;		vertices[64].y = -0.5f;			vertices[64].z = -0.5f;
	vertices[64].r = 255;			vertices[64].g = 255;			vertices[64].b = 255;
	vertices[64].u = 0.0f;			vertices[64].v = 0.0f;
	vertices[64].nx = 0.0f;			vertices[64].ny = 0.0f;			vertices[64].nz = -1.0f;

	vertices[65].x = -0.75f;		vertices[65].y = 0.5f;			vertices[65].z = -0.5f;
	vertices[65].r = 255;			vertices[65].g = 255;			vertices[65].b = 255;
	vertices[65].u = 0.0f;			vertices[65].v = 1.0f;
	vertices[65].nx = 0.0f;			vertices[65].ny = 0.0f;			vertices[65].nz = -1.0f;

	vertices[66].x = 0.75f;			vertices[66].y = 0.5f;			vertices[66].z = -0.5f;
	vertices[66].r = 255;			vertices[66].g = 255;			vertices[66].b = 255;
	vertices[66].u = 1.0f;			vertices[66].v = 1.0f;
	vertices[66].nx = 0.0f;			vertices[66].ny = 0.0f;			vertices[66].nz = -1.0f;

	vertices[67].x = 0.75f;			vertices[67].y = -0.5f;			vertices[67].z = -0.5f;
	vertices[67].r = 255;			vertices[67].g = 255;			vertices[67].b = 255;
	vertices[67].u = 1.0f;			vertices[67].v = 0.0f;
	vertices[67].nx = 0.0f;			vertices[67].ny = 0.0f;			vertices[67].nz = -1.0f;

	// Right Rectangle
	vertices[68].x = -0.75f;		vertices[68].y = -0.5f;			vertices[68].z = 0.0f;
	vertices[68].r = 255;			vertices[68].g = 255;			vertices[68].b = 255;
	vertices[68].u = 0.0f;			vertices[68].v = 0.0f;
	vertices[68].nx = -1.0f;		vertices[68].ny = 0.0f;			vertices[68].nz = 0.0f;

	vertices[69].x = -0.75f;		vertices[69].y = 0.5f;			vertices[69].z = 0.0f;
	vertices[69].r = 255;			vertices[69].g = 255;			vertices[69].b = 255;
	vertices[69].u = 0.0f;			vertices[69].v = 1.0f;
	vertices[69].nx = -1.0f;		vertices[69].ny = 0.0f;			vertices[69].nz = 0.0f;

	vertices[70].x = -0.75f;		vertices[70].y = 0.5f;			vertices[70].z = -0.5f;
	vertices[70].r = 255;			vertices[70].g = 255;			vertices[70].b = 255;
	vertices[70].u = 1.0f;			vertices[70].v = 1.0f;
	vertices[70].nx = -1.0f;		vertices[70].ny = 0.0f;			vertices[70].nz = 0.0f;

	vertices[71].x = -0.75f;		vertices[71].y = -0.5f;			vertices[71].z = -0.5f;
	vertices[71].r = 255;			vertices[71].g = 255;			vertices[71].b = 255;
	vertices[71].u = 1.0f;			vertices[71].v = 0.0f;
	vertices[71].nx = -1.0f;		vertices[71].ny = 0.0f;			vertices[71].nz = 0.0f;

	// Left Rectangle
	vertices[72].x = 0.75f;			vertices[72].y = -0.5f;			vertices[72].z = -0.5f;
	vertices[72].r = 255;			vertices[72].g = 255;			vertices[72].b = 255;
	vertices[72].u = 0.0f;			vertices[72].v = 0.0f;
	vertices[72].nx = 1.0f;			vertices[72].ny = 0.0f;			vertices[72].nz = 0.0f;

	vertices[73].x = 0.75f;			vertices[73].y = 0.5f;			vertices[73].z = -0.5f;
	vertices[73].r = 255;			vertices[73].g = 255;			vertices[73].b = 255;
	vertices[73].u = 0.0f;			vertices[73].v = 1.0f;
	vertices[73].nx = 1.0f;			vertices[73].ny = 0.0f;			vertices[73].nz = 0.0f;

	vertices[74].x = 0.75f;			vertices[74].y = 0.5f;			vertices[74].z = 0.0f;
	vertices[74].r = 255;			vertices[74].g = 255;			vertices[74].b = 255;
	vertices[74].u = 1.0f;			vertices[74].v = 1.0f;
	vertices[74].nx = 1.0f;			vertices[74].ny = 0.0f;			vertices[74].nz = 0.0f;

	vertices[75].x = 0.75f;			vertices[75].y = -0.5f;			vertices[75].z = 0.0f;
	vertices[75].r = 255;			vertices[75].g = 255;			vertices[75].b = 255;
	vertices[75].u = 1.0f;			vertices[75].v = 0.0f;
	vertices[75].nx = 1.0f;			vertices[75].ny = 0.0f;			vertices[75].nz = 0.0f;

	// Top Rectangle
	vertices[76].x = -0.75f;		vertices[76].y = 0.5f;			vertices[76].z = -0.5f;
	vertices[76].r = 255;			vertices[76].g = 255;			vertices[76].b = 255;
	vertices[76].u = 0.0f;			vertices[76].v = 0.0f;
	vertices[76].nx = 0.0f;			vertices[76].ny = 1.0f;			vertices[76].nz = 0.0f;

	vertices[77].x = -0.75f;		vertices[77].y = 0.5f;			vertices[77].z = 0.0f;
	vertices[77].r = 255;			vertices[77].g = 255;			vertices[77].b = 255;
	vertices[77].u = 0.0f;			vertices[77].v = 1.0f;	
	vertices[77].nx = 0.0f;			vertices[77].ny = 1.0f;			vertices[77].nz = 0.0f;

	vertices[78].x = 0.75f;			vertices[78].y = 0.5f;			vertices[78].z = 0.0f;
	vertices[78].r = 255;			vertices[78].g = 255;			vertices[78].b = 255;
	vertices[78].u = 1.0f;			vertices[78].v = 1.0f;
	vertices[78].nx = 0.0f;			vertices[78].ny = 1.0f;			vertices[78].nz = 0.0f;

	vertices[79].x = 0.75f;			vertices[79].y = 0.5f;			vertices[79].z = -0.5f;
	vertices[79].r = 255;			vertices[79].g = 255;			vertices[79].b = 255;
	vertices[79].u = 1.0f;			vertices[79].v = 0.0f;
	vertices[79].nx = 0.0f;			vertices[79].ny = 1.0f;			vertices[79].nz = 0.0f;

	// Bottom Rectangle
	vertices[80].x = 0.75f;			vertices[80].y = -0.5f;			vertices[80].z = -0.5f;
	vertices[80].r = 255;			vertices[80].g = 255;			vertices[80].b = 255;
	vertices[80].u = 0.0f;			vertices[80].v = 0.0f;
	vertices[80].nx = 0.0f;			vertices[80].ny = -1.0f;		vertices[80].nz = 0.0f;

	vertices[81].x = 0.75f;			vertices[81].y = -0.5f;			vertices[81].z = 0.0f;
	vertices[81].r = 255;			vertices[81].g = 255;			vertices[81].b = 255;
	vertices[81].u = 0.0f;			vertices[81].v = 1.0f;
	vertices[81].nx = 0.0f;			vertices[81].ny = -1.0f;		vertices[81].nz = 0.0f;

	vertices[82].x = -0.75f;		vertices[82].y = -0.5f;			vertices[82].z = 0.0f;
	vertices[82].r = 255;			vertices[82].g = 255;			vertices[82].b = 255;
	vertices[82].u = 1.0f;			vertices[82].v = 1.0f;
	vertices[82].nx = 0.0f;			vertices[82].ny = -1.0f;		vertices[82].nz = 0.0f;

	vertices[83].x = -0.75f;		vertices[83].y = -0.5f;			vertices[83].z = -0.5f;
	vertices[83].r = 255;			vertices[83].g = 255;			vertices[83].b = 255;
	vertices[83].u = 1.0f;			vertices[83].v = 0.0f;
	vertices[83].nx = 0.0f;			vertices[83].ny = -1.0f;		vertices[83].nz = 0.0f;

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

	// Textures
	
	// --- Room Textures ---

	// Create a variable that will contain the ID for our texture,
	// and use glGenTextures() to generate the texture itself
	GLuint tex0;
	glGenTextures(1, &tex0);

	// --- Load our image using stb_image ---

	// Im image-space (pixels), (0, 0) is the upper-left corner of the image
	// However, in u-v coordinates, (0, 0) is the lower-left corner of the image
	// This means that the image will appear upside-down when we use the image data as is
	// This function tells stbi to flip the image vertically so that it is not upside-down when we use it
	stbi_set_flip_vertically_on_load(true);

	// 'imageWidth' and imageHeight will contain the width and height of the loaded image respectively
	int imageWidth, imageHeight, numChannels;

	// Read the image data and store it in an unsigned char array
	unsigned char* imageData = stbi_load("CubeMap-FrontWall.png", &imageWidth, &imageHeight, &numChannels, 0);

	// Make sure that we actually loaded the image before uploading the data to the GPU
	if (imageData != nullptr)
	{
		// Our texture is 2D, so we bind our texture to the GL_TEXTURE_2D target
		glBindTexture(GL_TEXTURE_2D, tex0);

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

	GLuint tex1;
	glGenTextures(1, &tex1);

	imageData = stbi_load("CubeMap-BackWall.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex2;
	glGenTextures(1, &tex2);

	imageData = stbi_load("CubeMap-LeftRightWall.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex3;
	glGenTextures(1, &tex3);

	imageData = stbi_load("CubeMap-Ceiling.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex3);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex4;
	glGenTextures(1, &tex4);

	imageData = stbi_load("CubeMap-Floor.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	// --- Platform Texture ---

	GLuint tex5;
	glGenTextures(1, &tex5);

	imageData = stbi_load("PLATFORM-Wood.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex5);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	// --- Painting Textures ---

	GLuint tex6;
	glGenTextures(1, &tex6);

	imageData = stbi_load("PAINTING-Mona-Lisa.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex6);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex7;
	glGenTextures(1, &tex7);

	imageData = stbi_load("PAINTING-The-Starry-Night.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex7);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex8;
	glGenTextures(1, &tex8);

	imageData = stbi_load("PAINTING-The-Great-Wave-off-Kanagawa.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex8);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex9;
	glGenTextures(1, &tex9);

	imageData = stbi_load("PAINTING-The-Birth-of-Venus.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex9);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex10;
	glGenTextures(1, &tex10);

	imageData = stbi_load("PAINTING-Girl-with-a-Pearl-Earring.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex10);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex11;
	glGenTextures(1, &tex11);

	imageData = stbi_load("PAINTING-The-Scream.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex11);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
		imageData = nullptr;
	}
	else
	{
		std::cerr << "Failed to load image" << std::endl;
	}

	GLuint tex12;
	glGenTextures(1, &tex12);

	imageData = stbi_load("PAINTING-Frame.png", &imageWidth, &imageHeight, &numChannels, 0);

	if (imageData != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, tex12);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);

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
		glUniform3f(lightPositionUniformLocation, 0.0f, 0.0f, 0.0f);

		GLint lightAmbientUniformLocation = glGetUniformLocation(program, "lightAmbient");
		glUniform3f(lightAmbientUniformLocation, 0.2f, 0.2f, 0.2f);

		GLint lightDiffuseUniformLocation = glGetUniformLocation(program, "lightDiffuse");
		glUniform3f(lightDiffuseUniformLocation, 0.8f, 0.8f, 0.8f);

		GLint lightSpecularUniformLocation = glGetUniformLocation(program, "lightSpecular");
		glUniform3f(lightSpecularUniformLocation, 0.5f, 0.5f, 0.5f);

		// Uniform variables for spot light
		GLint spotlightPosition0UniformLocation = glGetUniformLocation(program, "spotlightPosition[0]");
		glUniform3f(spotlightPosition0UniformLocation, -10.0f, 20.0f, 10.0f);

		GLint spotlightPosition1UniformLocation = glGetUniformLocation(program, "spotlightPosition[1]");
		glUniform3f(spotlightPosition1UniformLocation, 10.0f, 20.0f, 10.0f);

		GLint spotlightPosition2UniformLocation = glGetUniformLocation(program, "spotlightPosition[2]");
		glUniform3f(spotlightPosition2UniformLocation, -10.0f, 20.0f, -10.0f);

		GLint spotlightPosition3UniformLocation = glGetUniformLocation(program, "spotlightPosition[3]");
		glUniform3f(spotlightPosition3UniformLocation, 10.0f, 20.0f, -10.0f);

		GLint spotlightAmbientUniformLocation = glGetUniformLocation(program, "spotlightAmbient");
		glUniform3f(spotlightAmbientUniformLocation, 0.2f, 0.2f, 0.1f);

		GLint spotlightDiffuseUniformLocation = glGetUniformLocation(program, "spotlightDiffuse");
		glUniform3f(spotlightDiffuseUniformLocation, 0.8f, 0.8f, 0.4f);

		GLint spotlightSpecularUniformLocation = glGetUniformLocation(program, "spotlightSpecular");
		glUniform3f(spotlightSpecularUniformLocation, 0.5f, 0.5f, 0.5f);

		GLint spotlightTargetUniformLocation = glGetUniformLocation(program, "spotlightTarget");
		glUniform3f(spotlightTargetUniformLocation, 0.0f, -1.0f, 0.0f);

		GLint spotlightCutoffUniformLocation = glGetUniformLocation(program, "spotlightCutoff");
		glUniform1f(spotlightCutoffUniformLocation, glm::cos(glm::radians(7.5f)));

		// Uniform variables for object
		GLint objectSpecularUniformLocation = glGetUniformLocation(program, "objectSpecular");
		glUniform3f(objectSpecularUniformLocation, 0.5f, 0.5f, 0.5f);

		GLint shininessUniformLocation = glGetUniformLocation(program, "shininess");
		glUniform1f(shininessUniformLocation, 8.0f);

		// --- Projection and View Matrices ---

		// Projection Matrix
		glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

		// View Matrix
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

		// Uniform variables
		GLint projUniformLocation = glGetUniformLocation(program, "proj");
		glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, glm::value_ptr(proj));

		GLint viewUniformLocation = glGetUniformLocation(program, "view");
		glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, glm::value_ptr(view));

		GLint cameraPositionUniformLocation = glGetUniformLocation(program, "cameraPosition");
		glUniform3fv(cameraPositionUniformLocation, 3, glm::value_ptr(cameraPosition));

		// --- Room ---

		// Model Matrix
		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 50.0f, 50.0f));

		// Normal Matrix
		glm::mat4 normal = glm::transpose(glm::inverse(model));

		// Uniform variables
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

		// --- Stand 1 ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -21.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 40, 4);

		// --- Stand 2 ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -21.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 40, 4);

		// --- Stand 3 ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, -21.0f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 40, 4);

		// --- Stand 4 ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -21.0f, -10.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 24, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 28, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 32, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 36, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 40, 4);

		// --- Painting 1: Solo Vertical ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 24.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(17.5f, 17.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 64, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 68, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 72, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 76, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 80, 4);

		// --- Painting 2: Solo Horizontal ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -24.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.5f, 17.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 64, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 68, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 72, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 76, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 80, 4);

		// --- Paintings 3 and 4: Horizontal and Square ---

		// --- Horizontal Painting ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, 7.5f, 5.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 64, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 68, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 72, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 76, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 80, 4);

		// --- Square Painting ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-24.0f, -7.5f, -7.5f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 44, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 48, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 52, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 56, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 60, 4);

		// --- Paintings 5 and 6: Vertical and Square ---

		// --- Vertical Painting ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(24.0f, 5.0f, -7.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 64, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 68, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 72, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 76, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 80, 4);

		// --- Square Painting ---

		// Model Matrix
		model = glm::translate(glm::mat4(1.0f), glm::vec3(24.0f, -7.5f, 7.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.5f, 12.5f, 2.0f));

		// Normal Matrix
		normal = glm::transpose(glm::inverse(model));

		// Uniform variables
		modelUniformLocation = glGetUniformLocation(program, "model");
		glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, glm::value_ptr(model));

		normMatrixUniformLocation = glGetUniformLocation(program, "normMatrix");
		glUniformMatrix4fv(normMatrixUniformLocation, 1, GL_FALSE, glm::value_ptr(normal));

		// Draw the vertices using triangle primitives
		glDrawArrays(GL_TRIANGLE_FAN, 44, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 48, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 52, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 56, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 60, 4);

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

/// <summary>
/// Function for handling the event when a key is pressed.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="key">Keyboard key</param>
/// <param name="scancode">Platform-specific scancode</param>
/// <param name="action">Action done on key</param>
/// <param name="mods">Modifier bits</param>
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	const float cameraSpeed = 0.25f; // adjust accordingly
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed / 2.0f);
	}
	else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * (cameraSpeed / 2.0f);
	}
	else if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
		cameraPosition += cameraSpeed * cameraFront;
	}
	else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
		cameraPosition -= cameraSpeed * cameraFront;
	}

	// Restricting camera movement within room's boundaries
	if (abs(cameraPosition.x) > 23.0f) cameraPosition.x = cameraPosition.x / abs(cameraPosition.x) * 23.0f;
	if (cameraPosition.y != -15.0f) cameraPosition.y = -15.0f;
	if (abs(cameraPosition.z) > 23.0f) cameraPosition.z = cameraPosition.z / abs(cameraPosition.z) * 23.0f;

	// --- Handling collisions with 3D model Stands ---
	float xDiff = abs(cameraPosition.x - previousX);
	float zDiff = abs(cameraPosition.z - previousZ);

	// Stand 1
	if ((cameraPosition.x <= -7.5f && cameraPosition.x >= -12.5f) && (cameraPosition.z >= 7.5f && cameraPosition.z <= 12.5f)) {
		if (abs(cameraPosition.x - (-7.5f)) < abs(cameraPosition.x - (-12.5f)) && xDiff > 0.12f)
			cameraPosition.x = -7.5f;
		else if (abs(cameraPosition.x - (-7.5f)) > abs(cameraPosition.x - (-12.5f)) && xDiff > 0.12f)
			cameraPosition.x = -12.5f;
		else if (abs(cameraPosition.z - (7.5f)) < abs(cameraPosition.z - (12.5f)) && zDiff > 0.12f)
			cameraPosition.z = 7.5f;
		else if (abs(cameraPosition.z - (7.5f)) > abs(cameraPosition.z - (12.5f)) && zDiff > 0.12f)
			cameraPosition.z = 12.5f;
	}

	// Stand 2
	if ((cameraPosition.x >= 7.5f && cameraPosition.x <= 12.5f) && (cameraPosition.z >= 7.5f && cameraPosition.z <= 12.5f)) {
		if (abs(cameraPosition.x - (7.5f)) < abs(cameraPosition.x - (12.5f)) && xDiff > 0.12f)
			cameraPosition.x = 7.5f;
		else if (abs(cameraPosition.x - (7.5f)) > abs(cameraPosition.x - (12.5f)) && xDiff > 0.12f)
			cameraPosition.x = 12.5f;
		else if (abs(cameraPosition.z - (7.5f)) < abs(cameraPosition.z - (12.5f)) && zDiff > 0.12f)
			cameraPosition.z = 7.5f;
		else if (abs(cameraPosition.z - (7.5f)) > abs(cameraPosition.z - (12.5f)) && zDiff > 0.12f)
			cameraPosition.z = 12.5f;
	}

	// Stand 3
	if ((cameraPosition.x <= -7.5f && cameraPosition.x >= -12.5f) && (cameraPosition.z <= -7.5f && cameraPosition.z >= -12.5f)) {
		if (abs(cameraPosition.x - (-7.5f)) < abs(cameraPosition.x - (-12.5f)) && xDiff > 0.12f)
			cameraPosition.x = -7.5f;
		else if (abs(cameraPosition.x - (-7.5f)) > abs(cameraPosition.x - (-12.5f)) && xDiff > 0.12f)
			cameraPosition.x = -12.5f;
		else if (abs(cameraPosition.z - (-7.5f)) < abs(cameraPosition.z - (-12.5f)) && zDiff > 0.12f)
			cameraPosition.z = -7.5f;
		else if (abs(cameraPosition.z - (-7.5f)) > abs(cameraPosition.z - (-12.5f)) && zDiff > 0.12f)
			cameraPosition.z = -12.5f;
	}

	// Stand 4
	if ((cameraPosition.x >= 7.5f && cameraPosition.x <= 12.5f) && (cameraPosition.z <= -7.5f && cameraPosition.z >= -12.5f)) {
		if (abs(cameraPosition.x - (7.5f)) < abs(cameraPosition.x - (12.5f)) && xDiff > 0.12f)
			cameraPosition.x = 7.5f;
		else if (abs(cameraPosition.x - (7.5f)) > abs(cameraPosition.x - (12.5f)) && xDiff > 0.12f)
			cameraPosition.x = 12.5f;
		else if (abs(cameraPosition.z - (-7.5f)) < abs(cameraPosition.z - (-12.5f)) && zDiff > 0.12f)
			cameraPosition.z = -7.5f;
		else if (abs(cameraPosition.z - (-7.5f)) > abs(cameraPosition.z - (-12.5f)) && zDiff > 0.12f)
			cameraPosition.z = -12.5f;
	}

	previousX = cameraPosition.x;
	previousZ = cameraPosition.z;

	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		// Tell OpenGL to display the cursor if it is hidden and vice versa
		if (!visibleCursor) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			visibleCursor = true;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			visibleCursor = false;
		}
	}
}

/// <summary>
/// Function for handling the event when the mouse moves.
/// </summary>
/// <param name="window">Reference to the window</param>
/// <param name="xpos">Mouse current x-position</param>
/// <param name="ypos">Mouse current y-position</param>
void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}