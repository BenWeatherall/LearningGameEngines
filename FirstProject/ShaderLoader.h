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

// Constants
const std::regex VERT_EXT(".*\.vert");
const std::regex FRAG_EXT(".*\.frag");

class ShaderLoader {
public:
	ShaderLoader();
	~ShaderLoader();
	void add_shaders(std::vector<std::string> filenames);
	GLuint build_program(std::vector<std::string> filenames);
private:

	std::map<std::string, GLuint>* built_shaders;

	void load_shader(std::string filename);

	bool is_shader_built(std::string);

	GLuint build_shader(GLchar** SourceCode, GLuint type);
	GLuint build_shader_program(std::vector<std::string> filenames);
};
