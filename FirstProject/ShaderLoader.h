#pragma once
#if _WIN64 || _WIN32
#include <Windows.h>
#elif linux
#include <sys/types.h>
#include <dirent.h>
#endif // _WIN32 || _WIN64

#include <fstream>
#include <string>
#include <cerrno>
#include <regex>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GLEW\include\GL\glew.h>
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