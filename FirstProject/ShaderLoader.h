#pragma once

#include <fstream>
#include <string>
#include <cerrno>
#include <regex>
#include <iostream>
#include <vector>
#include <map>

#include "File_IO.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// STRUCTS
/* Basic Shape Data*/
struct shaders {
	std::vector<GLuint> fragmentShader;
	std::vector<GLuint> vertexShader;
};

// Function Interface
shaders load_shaders(std::vector<std::string> vertex_shader, std::vector<std::string> fragment_shader);
GLuint BuildShader(GLchar** SourceCode, GLuint type);
GLuint BuildShaderProgram(std::vector<std::string> vertex_filenames, std::vector<std::string> fragment_filenames);
