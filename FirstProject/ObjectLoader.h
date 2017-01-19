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
};

/* Buffer / Array data for shapes stored on GPU */
struct VertexedShape {
	std::string name;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint vertices;
	// TODO Carry shape name should receive from coordinates
};

// Function Interface
std::vector<coordinates*>* load_shapes();

class LoadedVertexObjects {
public:
	LoadedVertexObjects(std::string);
	~LoadedVertexObjects();
	// Consider merging with constructor. 
	// Expected behaviour would be to add contents of supplied dir to existing file. 
	// Currently I believe this replaces it! Need to assess.
	void LoadDirectory(std::string); 

	std::vector<VertexedShape*> * GetShapes();

private:
	std::vector<coordinates*> * Coords; // Intended to be emptied after every LoadDirectory operation
	std::vector<VertexedShape*> * VertexObjects;

	std::vector<std::string> DirectoryContents(std::string);
	void BuildVertexObjects();
};