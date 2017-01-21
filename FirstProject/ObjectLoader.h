#pragma once

#include <cctype>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>

#include "File_IO.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// STRUCTS
/* Basic Shape Data*/
struct coordinates {
	std::string Name;
	std::vector<GLfloat>* VertContainer;
	GLfloat *VertData; // Pointer to array within VertContainer
	size_t VertSize; // Number of elements
};

struct elements {
	std::string Name;
	std::vector<GLuint>* ElemContainer;
	GLuint *ElemData; // Pointer to array within ElemContainer
	size_t ElemSize; // Number of elements
};

/* Buffer / Array data for shapes stored on GPU */
struct VertexedShape {
	std::string name;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint elements;
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
	std::vector<elements*> * Elems; // Intended to be emptied after every LoadDirectory operation
	std::vector<VertexedShape*> * VertexObjects;

	void BuildVertexObjects();
};
