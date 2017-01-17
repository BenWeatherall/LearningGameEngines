#pragma once
#if _WIN64 || _WIN32
	#include <Windows.h>
#elif linux
	#include <sys/types.h>
	#include <dirent.h>
#endif // _WIN32 || _WIN64

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GLEW\include\GL\glew.h>
// GLFW
#include <GLFW/glfw3.h>

// STRUCTS
/* Basic Shape Data*/
struct coordinates {
	std::vector<GLfloat>* container;
	GLfloat *data;
	size_t size;
	// TODO Carry shape name (Either via filename, OR Use first line of file for name
};

/* Buffer / Array data for shapes stored on GPU */
struct VertexedShapes {
	GLuint VAO;
	GLuint VBO;
	GLuint vertices;
	// TODO Carry shape name should receive from coordinates
};

// Function Interface
std::vector<coordinates*>* load_shapes();