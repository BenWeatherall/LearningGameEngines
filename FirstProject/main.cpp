// LEARNING BRANCH
#include <iostream>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "StaticMeshLoader.h"
#include "ShaderLoader.h"
#include "Seconds_Per_Frame_Counter.h"
#include "Camera.h"

// Window Dimensions
const GLuint WIDTH = 1024, HEIGHT = 768;

// Function Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	// Init GLFW
	glfwInit();
	// Set GLFW req options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	/* We can create as many windows as we like via this method; remember that each window will need to be made 
	current context to have actions applied to it, so really you should have a pointer / function that can be
	reused to perform the same actions if you want multiple windows.*/ 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Complex Mesh Project", nullptr, nullptr); // Window 1
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// I.E This is where the following actions shall occur
	glfwMakeContextCurrent(window);

	// Set required callbacks: Guessing this becomes a large switch which will then be converted into a hash (after all you always want rebindable keys!)
	// Following from that; I wonder how one does context... State machine?
	glfwSetKeyCallback(window, key_callback);

	// Initialise GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	// Set up viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	/*	TODO: Replace Shaders and Shapes with load operation for a csv file
		which will load specified meshes, their relevant shaders, offsets and rotations
	*/
	// Load Shaders into program
	GLuint ShaderProgram = BuildShaderProgram();
		// Load Shapes
	StaticMeshLoader MeshData("./Shapes/");


	// Initialise Seconds per Frame counter
	SPF_Counter spf_report = SPF_Counter();

	// Program Loop
	while (!glfwWindowShouldClose(window))
	{
		// Show current time per frame
		spf_report.tick();

		// Check and call events
		glfwPollEvents();

		// Rendering Commands here
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		GLfloat timeValue = glfwGetTime();

		GLint shaderTimer = glGetUniformLocation(ShaderProgram, "time");
		
		/*	TODO: Implement a 'view object' system. 
			On KeyPress (Say <ENTER>) call a new thread 
			This thread lists files loaded and askes which asks for input
			of which to load. On load the current object is replaced with
			the selected object and the thread dies
		*/
		glUseProgram(ShaderProgram);

		glUniform1f(shaderTimer, timeValue);

		for (unsigned int i = 0; i < MeshData.GetShapes()->size(); ++i) {
			glBindVertexArray(MeshData.GetShapes()->at(i)->VAO);
			glDrawElements(GL_TRIANGLES, MeshData.GetShapes()->at(i)->elements, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		// Swap Buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When user presses the escape key, we set the windowShouldClose property to true,
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
