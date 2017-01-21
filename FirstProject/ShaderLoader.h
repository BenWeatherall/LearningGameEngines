#pragma once

#include <fstream>
#include <string>
#include <cerrno>
#include <regex>
#include <iostream>

#include "File_IO.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// STRUCTS
/* Basic Shape Data*/
struct shaders {
	GLuint fragmentShader;
	GLuint vertexShader;
};

// Function Interface
shaders* load_shaders();
void BuildVertexShader(GLuint * VertexShader, GLchar** SourceCode);
void BuildFragmentShader(GLuint * FragmentShader, GLchar** SourceCode);
GLuint BuildShaderProgram();
