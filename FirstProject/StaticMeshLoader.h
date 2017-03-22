#pragma once

#include <cctype>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>

#include "File_IO.h"
#include "ShaderLoader.h"
#include "StaticMesh.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>

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
// TODO Remove and replace with Mesh class
struct StaticMesh {
	std::string name;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint elements;
	GLuint shader_program;
	glm::vec3 rotation;
	glm::vec3 location;
};

// Function Interface
// std::vector<coordinates*>* load_shapes();

class StaticMeshLoader {
public:
	StaticMeshLoader(std::string);
	~StaticMeshLoader();
	// Consider merging with constructor. 
	// Expected behaviour would be to add contents of supplied dir to existing file. 
	// Currently I believe this replaces it! Need to assess.
	void LoadDirectory(std::string); 

	std::vector<StaticMesh*> * GetShapes();

private:
	std::vector<coordinates*> * Coords; // Intended to be emptied after every LoadDirectory operation
	std::vector<elements*> * Elems; // Intended to be emptied after every LoadDirectory operation
	std::vector<StaticMesh*> * VertexObjects;

	void BuildVertexObjects();
};
